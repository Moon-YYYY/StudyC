cmake_minimum_required(VERSION 3.19)
project(CalculationTools LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network)

qt_standard_project_setup()

qt_add_executable(CalculationTools
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    CircleButton.h
    CircleButton.cpp
    CalculatorWindow.h
    CalculatorWindow.cpp
    calculateWay.h
    calculateWay.cpp
    menuButton.h
    menuButton.cpp
    UpdateChecker.h
    UpdateChecker.cpp
    UpdateDialog.h
    UpdateDialog.cpp
    ApkDownloader.h
    ApkDownloader.cpp
    SettingsWidget.h
    SettingsWidget.cpp
)

set_target_properties(CalculationTools PROPERTIES
   QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")

# Android: APK 签名配置（使用与 USB 调试相同的 debug.keystore）
# 确保 Debug 和 Release 使用同一签名，避免 OTA 更新时签名冲突
#
# 重要：必须放在 qt_add_executable 之后，且不能加条件判断
# Qt 不会自动设置 QT_ANDROID_SIGN_APK 变量，这里直接无条件启用
#
if(ANDROID)
    # 检查签名文件是否存在
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/android/debug.keystore")
        set_target_properties(CalculationTools PROPERTIES
            QT_ANDROID_SIGN_APK TRUE
            QT_ANDROID_KEYSTORE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/android/debug.keystore"
            QT_ANDROID_KEYSTORE_ALIAS "androiddebugkey"
            QT_ANDROID_KEYSTORE_STORE_PASS "android"
            QT_ANDROID_KEYSTORE_KEY_PASS "android"
        )
        message(STATUS "Android: 启用 APK 签名，使用 ${CMAKE_CURRENT_SOURCE_DIR}/android/debug.keystore")
    else()
        message(WARNING "Android: 签名文件 debug.keystore 未找到！Release APK 将无法安装")
        message(WARNING "请将 ~/.android/debug.keystore 复制到 android/debug.keystore")
    endif()
endif()

# Android: Include OpenSSL for HTTPS support
# 需要手动下载 https://github.com/KDAB/android_openssl 并解压到项目目录的 android_openssl/ 文件夹下
if(ANDROID)
    set(ANDROID_OPENSSL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android_openssl")
    if(EXISTS ${ANDROID_OPENSSL_DIR}/android_openssl.cmake)
        include(${ANDROID_OPENSSL_DIR}/android_openssl.cmake)
        add_android_openssl_libraries(CalculationTools)

        # 强制将 OpenSSL 库打包到 APK 中
        # Qt 6.5+ 使用 QT_ANDROID_EXTRA_LIBS 属性指定需要打包的额外 .so 库
        # 必须在 qt_add_executable 之后设置
        set_property(TARGET CalculationTools APPEND PROPERTY
            QT_ANDROID_EXTRA_LIBS
            ${ANDROID_OPENSSL_DIR}/no-asm/ssl_3/${CMAKE_ANDROID_ARCH_ABI}/libcrypto_3.so
            ${ANDROID_OPENSSL_DIR}/no-asm/ssl_3/${CMAKE_ANDROID_ARCH_ABI}/libssl_3.so
        )
    else()
        message(WARNING "android_openssl not found at ${ANDROID_OPENSSL_DIR}. HTTPS will not work.")
    endif()
endif()

target_link_libraries(CalculationTools
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
)

install(TARGETS CalculationTools
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET CalculationTools
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
