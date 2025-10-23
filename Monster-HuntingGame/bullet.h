#pragma once
#include<SFML/Graphics.hpp>
#include"People.h"
using namespace sf;
class bullet {
public:
    bullet(float angle, Vector2f StartPosition, Vector2i& MousePosition);
    static Vector2f CalculatefirstPosition(Vector2i& MousePosition);
    void updataPosition();
    void draw(RenderWindow& win);
    bool isOutOfBounds() const;

private:
    RectangleShape weapon_of_bullet;//手枪子弹
    Vector2f bulletSpeed;//分速度
    Vector2f Position;//分里程，改进，不是分里程，而是子弹初始位置
    float bulletX;
    float bulletY;
    float Speed = 10;//子弹速度
};