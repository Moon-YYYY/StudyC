#pragma once
#include<SFML/Graphics.hpp>
#include<string>
using namespace sf;
//常量定义
const int win_Xsize = 1600;
const int win_Ysize = 900;
const double M_PI = 3.14159265358979323;
struct StateMachine {//模拟状态机
    int Myself_foot1 = 1;
    int Myself_foot2 = 1;
    int decide_LeftOrRight = 0;//向右为1，向左为2
    int FrameRate = 2;//更新率调节

    bool click_BeginGame = false;
    bool click_Help = false;
    bool click_Setting = false;
    bool click_Exit = false;
    bool IsAttack = false;//判断是否攻击
    bool GunAppear = false;
    bool MousePressed = true;
    bool Press_D = true;
    bool Press_A = true;
    bool IsMoving_Right = false;
    bool IsMoving_Left = false;
    short UI = 0;
    //计时器///////////////////////////////////////////////////////////////////////////////////
    //0控制帧率
    //1控制手枪出现的时长
    //2控制手枪子弹射速
    bool Time(int duration, int id) {
        static int startTime[10];
        int endTime = clock();
        if (endTime - startTime[id] > duration) {
            startTime[id] = endTime;
            return true;
        }
        else {
            return false;
        }
    }
    float Myself_foot_Speed1 = 0.5;
    float Myself_foot_Speed2 = 0.1;
    bool KaiMu_Keep = true;
};

struct Math {
    const float Myself_X = 700;//人物坐标
    const float Myself_Y = 580;
    Vector2f Myself_foot1_rate{ 0, 90 };
    Vector2f Myself_foot2_rate{ 65,  90 };
    Vector2f Myself_eye1{ Myself_X + 50, Myself_Y + 20 };
    Vector2f Myself_eye2{ Myself_X + 75, Myself_Y + 20 };
    Vector2f Myself_Mouth{ Myself_X + 60, Myself_Y + 70 };
};
extern StateMachine ST;
extern Math Pos;
float CalculateMouseAngle(Vector2i& Mouseposition);
class People {
    friend class MathDealFunction;
public:
    People();
    void Change_Myself_foot_Right();
    void draw(RenderWindow& win, Vector2i MousePosition);
private:
    CircleShape Myself;//人物的身体
    CircleShape Myself_foot1;//人物的脚
    CircleShape Myself_foot2;
    CircleShape Myself_eyes1;//人物眼睛
    CircleShape Myself_eyes2;
    CircleShape Myself_Mouth;//人物嘴巴
    Color Myself_Color = { 0, 0, 0, 0 };
    Vector2f Myself_foot1_Offset{ Pos.Myself_X + Pos.Myself_foot1_rate.x, Pos.Myself_Y + Pos.Myself_foot1_rate.y };//人物下肢1偏移量
    Vector2f Myself_foot2_Offset{ Pos.Myself_X + Pos.Myself_foot2_rate.x, Pos.Myself_Y + Pos.Myself_foot2_rate.y };//人物下肢2偏移量
};