#include <SFML/Graphics.hpp>
#include <iostream>
#include "ball.hpp"
#include "blocks.hpp"
#include "paddle.hpp"
#include "bonuce.hpp"

using namespace std;
using namespace sf;

int score{ 0 };

template <class T1, class T2>
bool isIntersecting(T1& mA, T2& mB)
{
    return mA.right() >= mB.left() && mA.left() <= mB.right() &&
        mA.bottom() >= mB.top() && mA.top() <= mB.bottom();
}

void testCollisionPB(Paddle& mPaddle, Ball& mBall)
{
    if (!isIntersecting(mPaddle, mBall)) return;

    mBall.velocity.y = -ballVelocity;
    if (mBall.x() < mPaddle.x())
        mBall.velocity.x = -ballVelocity;
    else
        mBall.velocity.x = ballVelocity;
}


void testCollisionBB(Brick& mBrick, Ball& mBall, Paddle& mPaddle)
{
    if (!isIntersecting(mBrick, mBall)) return;
    mBrick.hardness -= 1;
    if (mBrick.hardness == 0) mBrick.destroyed = true;
    score += 1;

    switch (mBrick.bonus) {
    case 's': {
        mBall.setVelocity(1.2);
        break;}
    case 'd': {
        //Ball ball2{ windowWidth / 2, windowHeight / 2 };
        Loot loot(mBrick.x(), mBrick.y());

        break;}
    case 'l':
        paddleWidth += 30.f;
        mPaddle.shape.setSize({ paddleWidth, paddleHeight });
        break;
    case 'b':

        break;
    case 'u':
        mBrick.destroyed = false;
        break;
    }

    float overlapLeft{ mBall.right() - mBrick.left() };
    float overlapRight{ mBrick.right() - mBall.left() };
    float overlapTop{ mBall.bottom() - mBrick.top() };
    float overlapBottom{ mBrick.bottom() - mBall.top() };

    bool ballFromLeft(abs(overlapLeft) < abs(overlapRight));
    bool ballFromTop(abs(overlapTop) < abs(overlapBottom));

    float minOverlapX{ ballFromLeft ? overlapLeft : overlapRight };
    float minOverlapY{ ballFromTop ? overlapTop : overlapBottom };

    if (abs(minOverlapX) < abs(minOverlapY))
        mBall.velocity.x = ballFromLeft ? -ballVelocity : ballVelocity;
    else
        mBall.velocity.y = ballFromTop ? -ballVelocity : ballVelocity;
}

bool IsTaken(Bonus* bonus, Paddle* paddle, Time gameTime) {
    if (!IsTouch(*bonus, *paddle)) {
        return false;
    }
    if (IsTouch(*bonus, *paddle)) {
        bonus->GetBonusClock().restart().asSeconds();
        bonus->SetActivity(true);
        return true;
    }
}