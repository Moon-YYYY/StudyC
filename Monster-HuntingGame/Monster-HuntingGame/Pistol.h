#pragma once
#include<SFML/Graphics.hpp>
#include<vector>
#include"bullet.h"
#include"Weapon.h"
using namespace sf;

class Pistol : public Weapon {
public:
    Pistol();
    void UpdataWeaponRotation(Vector2i& MousePosition);
    void updataBullets();
    void shoot(Vector2i& MousePosition);
    void draw(RenderWindow& win);
private:
    vector<bullet>  bullets;//×Óµ¯ÈÝÆ÷
    RectangleShape weapon_of_GunGrip;//ÊÖÇ¹ÎäÆ÷Ç¹°Ñ
    RectangleShape weapon_of_GunBarrel;//ÊÖÇ¹ÎäÆ÷Ç¹¹Ü
    Color Myself_Color = { 0, 0, 0, 0 };
};