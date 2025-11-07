#pragma once
#include<SFML/Graphics.hpp>
#include"People.h"
using namespace std;
class Weapon {
public:
    virtual void shoot(Vector2i& MousePosition) = 0;
    virtual void updataBullets() = 0;
    virtual void draw(RenderWindow& win) = 0;
    virtual void UpdataWeaponRotation(Vector2i& MousePosition) = 0;
    virtual ~Weapon() {}
private:
};