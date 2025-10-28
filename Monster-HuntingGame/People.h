#pragma once
#include<SFML/Graphics.hpp>
#include<string>
using namespace sf;
//��������
const int win_Xsize = 1600;
const int win_Ysize = 900;
const double M_PI = 3.14159265358979323;
struct StateMachine {//ģ��״̬��
    int Myself_foot1 = 1;
    int Myself_foot2 = 1;
    int decide_LeftOrRight = 0;//����Ϊ1������Ϊ2
    int FrameRate = 2;//�����ʵ���

    bool click_BeginGame = false;
    bool click_Help = false;
    bool click_Setting = false;
    bool click_Exit = false;
    bool IsAttack = false;//�ж��Ƿ񹥻�
    bool GunAppear = false;
    bool MousePressed = true;
    bool Press_D = true;
    bool Press_A = true;
    bool IsMoving_Right = false;
    bool IsMoving_Left = false;
    short UI = 0;
    //��ʱ��///////////////////////////////////////////////////////////////////////////////////
    //0����֡��
    //1������ǹ���ֵ�ʱ��
    //2������ǹ�ӵ�����
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
    const float Myself_X = 700;//��������
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
    CircleShape Myself;//���������
    CircleShape Myself_foot1;//����Ľ�
    CircleShape Myself_foot2;
    CircleShape Myself_eyes1;//�����۾�
    CircleShape Myself_eyes2;
    CircleShape Myself_Mouth;//�������
    Color Myself_Color = { 0, 0, 0, 0 };
    Vector2f Myself_foot1_Offset{ Pos.Myself_X + Pos.Myself_foot1_rate.x, Pos.Myself_Y + Pos.Myself_foot1_rate.y };//������֫1ƫ����
    Vector2f Myself_foot2_Offset{ Pos.Myself_X + Pos.Myself_foot2_rate.x, Pos.Myself_Y + Pos.Myself_foot2_rate.y };//������֫2ƫ����
};