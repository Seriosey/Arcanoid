#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "blocks.hpp"
#include "ball.hpp"
#include"window.hpp"
#include <vector>
#include "blocks.hpp"
#include"paddle.hpp"

using namespace std;
using namespace sf;

constexpr float bonusWidth{ 20.f }, bonusHeight{ 20.f }, bonusVelocity{ 0.3 };
const Time oneBonusTime = seconds(10);

typedef enum BonusType {
    NONE, 
    sizeIncreaseBonus,
    speedUpBonus,
    safeBottomBonus,
    stickCarriageBonus,
    changeWayBonus,
    newBlockBonus,
    secondBall
};

class Bonus : public Rectangle
{
public:
    Bonus(float x, float y);
    Vector2f velocity{ 0.f, 5.f };
    float block_pos_x;
    float block_pos_y;
    

    string type = "none";
    Clock bonusClock;
    Time startTime;
    bool isActive;

    string GetType() { return type; };
    void BonusCheck(Paddle* paddle, Ball* ball, RenderWindow* window, Time gameClock);
    Time GetStartTime() { return startTime; };
    void SetType(string _type) { type = _type; };
    void SetStartTime(Time _startTime) { startTime = _startTime; };
    void SetActivity(bool condition) { isActive = condition; };
    bool IsActive() { return isActive; };
    void StartTime() { Clock bonusClock; };
    Clock GetBonusClock() { return bonusClock; };
    virtual void BonusActivate() {};
    virtual void BonusDeactivate() {};

    void update();
    //virtual ~Bonus() = default;
};

class SizeIncreaseBonus : public Bonus {
public:
    SizeIncreaseBonus(float _x, float _y, Paddle* _paddle);
    void BonusActivate();
    void BonusDeactivate();

private:
    Paddle* paddle;
};

//class SpeedUpBonus : public Bonus {
//public:
//    SpeedUpBonus(float _x, float _y, Ball* _ball);
//    void BonusActivate();
//    void BonusDeactivate();
//private:
//    Ball* ball;
//};

class SafeBottomBonus : public Bonus {
public:
    SafeBottomBonus(float _x, float _y, RenderWindow* _window, Ball* _ball);
    void BonusActivate();
private:
    RenderWindow* window;
    Ball* ball;
};

class StickCarriageBonus : public Bonus {
public:
    StickCarriageBonus(float _x, float _y, Paddle* _paddle, Ball* _ball);
    void BonusActivate();
private:
    Paddle* paddle;
    Ball* ball;
};

class ChangeWayBonus : public Bonus {
public:
    ChangeWayBonus(float _x, float _y, Ball* _ball);
    void SetRandTime() { randTime = rand() % (int)oneBonusTime.asSeconds(); }
    int GetRandTime() { return randTime; };
    void BonusActivate();
    void BonusDeactivate();
private:
    int randTime;
    Ball* ball;
};

class SecondBall : public Bonus {
public:
    SecondBall(float _x, float _y, RenderWindow* _window, Ball* _ball, vector<Brick> _bricks, Paddle* _paddle);
    void BonusActivate();
    void BonusDeactivate();
private:
    vector<Brick> bricks;
    RenderWindow* window;
    Ball* second_ball;
    Ball* ball;
    Paddle* paddle;
};

class NewBlockBonus : public Bonus {
public:
    NewBlockBonus(float _x, float _y, Ball* _ball, RenderWindow* _window, Paddle* _paddle);
    void BonusActivate();

private:
    MovingBlock* block;
    RenderWindow* window;
    Ball* ball;
    Paddle* paddle;
    vector<MovingBlock*> blocks;
};

Bonus* CreateBonuses(float mx, float my, Paddle* paddle, Ball* ball, RenderWindow* window);
