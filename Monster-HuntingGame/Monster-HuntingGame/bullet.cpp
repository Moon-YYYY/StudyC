#include"bullet.h"

bullet::bullet(float angle, Vector2f StartPosition, Vector2i& MousePosition) {
    weapon_of_bullet.setSize(Vector2f(10, 1));//子弹
    weapon_of_bullet.setFillColor(Color::Red);
    weapon_of_bullet.setOutlineThickness(2);
    weapon_of_bullet.setOutlineColor(Color::Red);
    weapon_of_bullet.setRotation(angle);
    weapon_of_bullet.setPosition(StartPosition);
    float dx = MousePosition.x - (Pos.Myself_X + 50);
    float dy = MousePosition.y - (Pos.Myself_Y + 50);
    float R = sqrt((dy * dy) + (dx * dx));
    bulletX = dx / R;
    bulletY = dy / R;
}
Vector2f bullet::CalculatefirstPosition(Vector2i& MousePosition) {//计算子弹的初始位置
    float dy = MousePosition.y - (Pos.Myself_Y + 50);
    float dx = MousePosition.x - (Pos.Myself_X + 50);
    float R = 35;
    Vector2f Position;
    float angle = CalculateMouseAngle(MousePosition);
    Position.x = Pos.Myself_X + 50 + cos(angle * (M_PI / 180.0)) * R;
    Position.y = Pos.Myself_Y + 50 + sin(angle * (M_PI / 180.0)) * R;
    return Position;
}
void bullet::updataPosition() {
    bulletSpeed.x += bulletX * Speed;
    bulletSpeed.y += bulletY * Speed;
    weapon_of_bullet.setPosition(bulletSpeed.x + (Pos.Myself_X + 50), bulletSpeed.y + (Pos.Myself_Y + 50));
}
void bullet::draw(RenderWindow& win) {
    win.draw(weapon_of_bullet);
}
bool bullet::isOutOfBounds() const {
    Vector2f pos = weapon_of_bullet.getPosition();
    return pos.x < 0 || pos.x > win_Xsize || pos.y < 0 || pos.y > win_Ysize;
}