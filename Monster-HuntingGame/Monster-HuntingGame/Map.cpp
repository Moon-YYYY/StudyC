#include"Map.h"

Map::Map() {
    Color Map_earth_Color = { 171, 113, 57, 255 };
    earth.setSize(Vector2f(3200.f, 500.f));
    earth.setPosition(0, 700);
    earth.setFillColor(Map_earth_Color);
}
void Map::draw(RenderWindow& win) {
    win.draw(earth);
}
