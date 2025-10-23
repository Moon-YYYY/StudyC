#include"People.h"
People::People() {
   
   Myself.setRadius(50.f);
    Myself.setFillColor(Myself_Color);
    Myself.setOutlineColor(Color::White);
    Myself.setOutlineThickness(2);
    Myself.setPosition(Pos.Myself_X, Pos.Myself_Y);

    Myself_foot1.setRadius(15.f);
    Myself_foot1.setFillColor(Color::White);
    Myself_foot1.setPosition(Myself_foot1_Offset.x, Myself_foot1_Offset.y);

    Myself_foot2.setRadius(15.f);
    Myself_foot2.setFillColor(Color::White);
    Myself_foot2.setPosition(Myself_foot2_Offset.x, Myself_foot2_Offset.y);

    Myself_eyes1.setFillColor(Color::White);
    Myself_eyes1.setPosition(Pos.Myself_eye1.x, Pos.Myself_eye1.y);
    Myself_eyes1.setRadius(7.f);

    Myself_eyes2.setFillColor(Color::White);
    Myself_eyes2.setPosition(Pos.Myself_eye2.x, Pos.Myself_eye2.y);
    Myself_eyes2.setRadius(7.f);

    Myself_Mouth.setFillColor(Color::White);
    Myself_Mouth.setPosition(Pos.Myself_Mouth);
    Myself_Mouth.setRadius(7.f);
    Myself_Mouth.setPointCount(3);

}
void People::Change_Myself_foot_Right() {
    if (ST.IsMoving_Right) {
        Myself_foot1_Offset.x = Pos.Myself_X + Pos.Myself_foot1_rate.x;
        Myself_foot1_Offset.y = Pos.Myself_Y + Pos.Myself_foot1_rate.y;

        Myself_foot2_Offset.x = Pos.Myself_X + Pos.Myself_foot2_rate.x;
        Myself_foot2_Offset.y = Pos.Myself_Y + Pos.Myself_foot2_rate.y;
        //下肢1运动函数
        switch (ST.Myself_foot1) {
        case 1:
            Pos.Myself_foot1_rate.x += ST.Myself_foot_Speed1;
            Pos.Myself_foot1_rate.y -= ST.Myself_foot_Speed2;
            Myself_foot1.setPosition(Myself_foot1_Offset.x, Myself_foot1_Offset.y);
            if (Pos.Myself_foot1_rate.x >= 65) {
                ST.Myself_foot1 = 2;
            }
            break;
        case 2:
            Pos.Myself_foot1_rate.y += ST.Myself_foot_Speed1;
            Myself_foot1.setPosition(Myself_foot1_Offset.x, Myself_foot1_Offset.y);
            if (Pos.Myself_foot1_rate.y >= 90) {
                ST.Myself_foot1 = 3;
            }
            break;
        case 3:
            Pos.Myself_foot1_rate.x -= ST.Myself_foot_Speed1;
            Myself_foot1.setPosition(Myself_foot1_Offset.x, Myself_foot1_Offset.y);
            if (Pos.Myself_foot1_rate.x <= 0) {
                ST.Myself_foot1 = 1;
            }
            break;

        default:
            break;
        }

        //下肢2运动函数
        switch (ST.Myself_foot2) {
        case 1:
            Pos.Myself_foot2_rate.x -= ST.Myself_foot_Speed1;
            Myself_foot2.setPosition(Myself_foot2_Offset.x, Myself_foot2_Offset.y);
            if (Pos.Myself_foot2_rate.x <= 0) {
                ST.Myself_foot2 = 2;
            }
            break;
        case 2:
            Pos.Myself_foot2_rate.x += ST.Myself_foot_Speed1;
            Pos.Myself_foot2_rate.y -= ST.Myself_foot_Speed2;
            Myself_foot2.setPosition(Myself_foot2_Offset.x, Myself_foot2_Offset.y);
            if (Pos.Myself_foot2_rate.x >= 65) {
                ST.Myself_foot2 = 3;
            }
            break;
        case 3:
            Pos.Myself_foot2_rate.y += ST.Myself_foot_Speed1;
            Myself_foot2.setPosition(Myself_foot2_Offset.x, Myself_foot2_Offset.y);
            if (Pos.Myself_foot2_rate.y >= 90) {
                ST.Myself_foot2 = 1;
            }
            break;
        default:
            break;
        }
    }
    else if (ST.IsMoving_Left) {
        Myself_foot1_Offset.x = Pos.Myself_X + Pos.Myself_foot1_rate.x;
        Myself_foot1_Offset.y = Pos.Myself_Y + Pos.Myself_foot1_rate.y;

        Myself_foot2_Offset.x = Pos.Myself_X + Pos.Myself_foot2_rate.x;
        Myself_foot2_Offset.y = Pos.Myself_Y + Pos.Myself_foot2_rate.y;
        switch (ST.Myself_foot1) {
        case 1://A键生效,向左运动
            Pos.Myself_foot1_rate.x += ST.Myself_foot_Speed1;
            Myself_foot1.setPosition(Myself_foot1_Offset.x, Myself_foot1_Offset.y);
            if (Pos.Myself_foot1_rate.x >= 65) {
                ST.Myself_foot1 = 2;
            }
            break;
        case 2:
            Pos.Myself_foot1_rate.x -= ST.Myself_foot_Speed1;
            Pos.Myself_foot1_rate.y -= ST.Myself_foot_Speed2;
            Myself_foot1.setPosition(Myself_foot1_Offset.x, Myself_foot1_Offset.y);
            if (Pos.Myself_foot1_rate.x <= 0) {
                ST.Myself_foot1 = 3;
            }
            break;
        case 3:
            Pos.Myself_foot1_rate.y += ST.Myself_foot_Speed1;
            Myself_foot1.setPosition(Myself_foot1_Offset.x, Myself_foot1_Offset.y);
            if (Pos.Myself_foot1_rate.y >= 90) {
                ST.Myself_foot1 = 1;
            }
            break;
        default:
            break;
        }
        switch (ST.Myself_foot2) {
        case 1://A按键生效，向左移动
            Pos.Myself_foot2_rate.x -= ST.Myself_foot_Speed1;
            Pos.Myself_foot2_rate.y -= ST.Myself_foot_Speed2;
            Myself_foot2.setPosition(Myself_foot2_Offset.x, Myself_foot2_Offset.y);
            if (Pos.Myself_foot2_rate.x <= 0) {
                ST.Myself_foot2 = 2;
            }
            break;
        case 2:
            Pos.Myself_foot2_rate.y += ST.Myself_foot_Speed1;
            Myself_foot2.setPosition(Myself_foot2_Offset.x, Myself_foot2_Offset.y);
            if (Pos.Myself_foot2_rate.y >= 90) {
                ST.Myself_foot2 = 3;
            }
            break;
        case 3:
            Pos.Myself_foot2_rate.x += ST.Myself_foot_Speed1;
            Myself_foot2.setPosition(Myself_foot2_Offset.x, Myself_foot2_Offset.y);
            if (Pos.Myself_foot2_rate.x >= 65) {
                ST.Myself_foot2 = 1;
            }
            break;
        default:
            break;
        }
    }
    else {

        Pos.Myself_foot1_rate.x = 0;
        Pos.Myself_foot1_rate.y = 90;
        Pos.Myself_foot2_rate.x = 65;
        Pos.Myself_foot2_rate.y = 90;
        Myself_foot1_Offset.x = Pos.Myself_X + Pos.Myself_foot1_rate.x;
        Myself_foot1_Offset.y = Pos.Myself_Y + Pos.Myself_foot1_rate.y;
        Myself_foot2_Offset.x = Pos.Myself_X + Pos.Myself_foot2_rate.x;
        Myself_foot2_Offset.y = Pos.Myself_Y + Pos.Myself_foot2_rate.y;
        Myself_foot1.setPosition(Myself_foot1_Offset);
        Myself_foot2.setPosition(Myself_foot2_Offset);
        ST.Myself_foot1 = 1;
        ST.Myself_foot2 = 1;
    }
}
void People::draw(RenderWindow& win, Vector2i MousePosition) {
    /////控制人物朝向
    if (MousePosition.x > Pos.Myself_X + 50) {
        Myself_eyes2.setPosition(Pos.Myself_eye2.x, Pos.Myself_eye2.y);
        Myself_Mouth.setPosition(Pos.Myself_Mouth.x, Pos.Myself_Mouth.y);
    }
    else {
        Myself_eyes2.setPosition(Pos.Myself_eye2.x - 50, Pos.Myself_eye2.y);
        Myself_Mouth.setPosition(Pos.Myself_Mouth.x - 20, Pos.Myself_Mouth.y);

    }
    win.draw(Myself);
    win.draw(Myself_foot1);
    win.draw(Myself_foot2);
    win.draw(Myself_eyes1);
    win.draw(Myself_eyes2);
    win.draw(Myself_Mouth);
}