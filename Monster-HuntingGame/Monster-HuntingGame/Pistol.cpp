#include"Pistol.h"
Pistol::Pistol() {
    weapon_of_GunGrip.setSize(Vector2f(5, 25));//Ç¹°Ñ
    weapon_of_GunGrip.setFillColor(Color::Yellow);
    weapon_of_GunGrip.setOutlineThickness(2);
    weapon_of_GunGrip.setOutlineColor(Color::Yellow);
    weapon_of_GunGrip.setPosition(Vector2f(Pos.Myself_X + 50, Pos.Myself_Y + 50));

    weapon_of_GunBarrel.setSize(Vector2f(35, 5));//Ç¹¹Ü
    weapon_of_GunBarrel.setFillColor(Myself_Color);
    weapon_of_GunBarrel.setOrigin(0, 2);
    weapon_of_GunBarrel.setOutlineThickness(2);
    weapon_of_GunBarrel.setOutlineColor(Color::Yellow);
    weapon_of_GunBarrel.setPosition(Vector2f(Pos.Myself_X + 50, Pos.Myself_Y + 50));
}
void Pistol::UpdataWeaponRotation(Vector2i& MousePosition) {
    if (MousePosition.x > Pos.Myself_X + 50) {
        weapon_of_GunGrip.setRotation(CalculateMouseAngle(MousePosition));
    }
    else {
        weapon_of_GunGrip.setRotation(CalculateMouseAngle(MousePosition) + 180.0);
    }
    weapon_of_GunBarrel.setRotation(CalculateMouseAngle(MousePosition));
}
void Pistol::updataBullets() {
    for (auto it = bullets.begin(); it != bullets.end();) {
        it->updataPosition();
        if (it->isOutOfBounds()) {
            it = bullets.erase(it);
        }
        else {
            ++it;
        }
    }
}
void Pistol::shoot(Vector2i& MousePosition) {
    bullets.emplace_back(CalculateMouseAngle(MousePosition), bullet::CalculatefirstPosition(MousePosition), MousePosition);
}
void Pistol::draw(RenderWindow& win) {
    switch (ST.GunAppear) {
    case true:
        win.draw(weapon_of_GunGrip);
        win.draw(weapon_of_GunBarrel);
        break;
    case false:
        break;
    }
    for (auto it = bullets.begin(); it != bullets.end(); it++) {
        it->draw(win);
    }
}