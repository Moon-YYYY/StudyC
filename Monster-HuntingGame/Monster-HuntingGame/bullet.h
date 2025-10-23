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
    RectangleShape weapon_of_bullet;//��ǹ�ӵ�
    Vector2f bulletSpeed;//���ٶ�
    Vector2f Position;//����̣��Ľ������Ƿ���̣������ӵ���ʼλ��
    float bulletX;
    float bulletY;
    float Speed = 10;//�ӵ��ٶ�
};