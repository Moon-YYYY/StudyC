#pragma once
#include<SFML/Graphics.hpp>
#include"People.h"
using namespace sf;
class KeyOrMouse {
public:
    KeyOrMouse();
    static bool isClick();
    void ShouQiang_IsAttack(Event& event);
    void People_Move(Event& event);
private:
};