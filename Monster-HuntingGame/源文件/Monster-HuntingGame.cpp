#include<vector>
#include<iostream>
#include<time.h>
#include<conio.h>
#include<cmath>
#include<SFML/Graphics.hpp>
#include<string>
#include<Windows.h>
#include"People.h"
#include"Map.h"
#include"bullet.h"
#include"Pistol.h"
#include"Weapon.h"
#include"KeyAndMouse.h"
#include"LoadTexture.h"
using namespace sf;
using namespace std;

StateMachine ST;
Math Pos;

float CalculateMouseAngle(Vector2i& Mouseposition) {//鼠标与原点角度
    float dx = Mouseposition.x - (Pos.Myself_X + 50);
    float dy = Mouseposition.y - (Pos.Myself_Y + 50);
    float angle = atan2(dy, dx) * (180.0f / static_cast<float>(M_PI));
    return angle;
}
int main() {
    RenderWindow win(VideoMode(win_Xsize, win_Ysize), "Monster-HundingGame");
    Map Map_earth;
    People People_Myself;
    Event event;
    KeyOrMouse KeyAndMouse;
    Weapon* DecideWeapon = nullptr;
    LoadTexture textures;
    if(DecideWeapon)
    delete DecideWeapon;
    DecideWeapon = new Pistol();
    while (win.isOpen()) {
        win.clear(Color::Black);
        Vector2i MousePosition = Mouse::getPosition(win);
        //float Y = People_Myself.Calculate(MousePosition);
        //cout << People_Myself.CalculateY(MousePosition) << ' ' << People_Myself.CalculateX(MousePosition, Y) << ' ' << endl;
        while (win.pollEvent(event)) {
            if (event.type == Event::Closed) {
                win.close();
            }
            KeyAndMouse.ShouQiang_IsAttack(event);
            KeyAndMouse.People_Move(event);
        }
        if (ST.Time(1000, 1)) {
            ST.GunAppear = false;
        }
        if (ST.Time(ST.FrameRate, 0)) {
            if (ST.IsAttack == true) {
                if (ST.Time(500, 2)) {
                    DecideWeapon->shoot(MousePosition);
                }
                ST.IsAttack = false;
                ST.MousePressed = false;
            }
            DecideWeapon->UpdataWeaponRotation(MousePosition);
            DecideWeapon->updataBullets();
            People_Myself.Change_Myself_foot_Right();
            switch (ST.UI) {
            case 0:
                textures.setCurrentUI(0);
                textures.Updata(MousePosition, event);
                textures.draw(win);
                
                break;
            case 1:
                textures.setCurrentUI(1);
                textures.Updata(MousePosition, event);
                textures.draw(win);
                break;
            case 5:
                DecideWeapon->draw(win);
                People_Myself.draw(win, MousePosition);
                Map_earth.draw(win);
                break;
            default:
                break;

            }
            
            win.display();
            if (ST.KaiMu_Keep) {
                Sleep(3000);
                ST.KaiMu_Keep = false;
            }
        }
    }
    delete DecideWeapon;
    return 0;
}