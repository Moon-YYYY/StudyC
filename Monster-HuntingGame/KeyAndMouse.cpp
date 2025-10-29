#include"KeyAndMouse.h"
KeyOrMouse::KeyOrMouse() {}
void KeyOrMouse::ShouQiang_IsAttack(Event& event) {
    //¿ØÖÆÊÖÇ¹ÎäÆ÷ÊÇ·ñ¹¥»÷
    if (ST.UI == 5) {
        if (ST.MousePressed) {
            if (Mouse::isButtonPressed(Mouse::Left)) {
                ST.IsAttack = true;
                ST.GunAppear = true;
            }
        }
        if (event.type == Event::MouseButtonReleased) {
            if (event.key.code == Mouse::Left)
                ST.MousePressed = true;
        }
    }
}
bool KeyOrMouse::isClick() {
    if (Mouse::isButtonPressed(Mouse::Left)) {
        return true;
    }
    return false;
}
bool KeyOrMouse::isMouseLeftRelease(Event& event) {
    if (event.type == Event::MouseButtonReleased) {
        if (event.key.code == Mouse::Left) {
            return true;
        }
    }
    else {
        return false;
    }
}
void KeyOrMouse::People_Move(Event& event) {
    if (Keyboard::isKeyPressed(Keyboard::D)) {//ÏòÓÒ
        ST.IsMoving_Right = true;
        ST.UI = 1;
    }
    if (event.type == Event::KeyReleased) {
        if (event.key.code == Keyboard::D) {
            ST.IsMoving_Right = false;
        }
    }
    if (Keyboard::isKeyPressed(Keyboard::A)) {//Ïò×ó
        ST.IsMoving_Left = true;
    }
    if (event.type == Event::KeyReleased) {
        if (event.key.code == Keyboard::A) {
            ST.IsMoving_Left = false;
        }
    }
}