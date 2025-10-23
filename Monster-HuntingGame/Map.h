#pragma once
#include<SFML/Graphics.hpp>
using namespace sf;
class Map {
public:
    Map();
    void draw(RenderWindow& win);

private:
    RectangleShape earth;
};