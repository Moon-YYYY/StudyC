#pragma once
#include<SFML/Graphics.hpp>
#include"People.h"
using namespace sf;
class KeyAndMouse {
public:
    KeyAndMouse();
    void ShouQiang_IsAttack(Event& event);
    void People_Move(Event& event);

};