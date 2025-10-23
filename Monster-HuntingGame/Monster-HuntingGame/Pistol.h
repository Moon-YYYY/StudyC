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
    vector<bullet>  bullets;//�ӵ�����
    RectangleShape weapon_of_GunGrip;//��ǹ����ǹ��
    RectangleShape weapon_of_GunBarrel;//��ǹ����ǹ��
    Color Myself_Color = { 0, 0, 0, 0 };
};