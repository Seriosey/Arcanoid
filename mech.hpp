#pragma once
#include "ball.hpp"
#include "blocks.hpp"
#include "paddle.hpp"
#include "bonuce.hpp"

using namespace std;
using namespace sf;

extern int score;
extern vector <Bonus*> bonuses;

template <class T1, class T2>
bool isIntersecting(T1& mA, T2& mB)
{
    return mA.right() >= mB.left() && mA.left() <= mB.right() &&
        mA.bottom() >= mB.top() && mA.top() <= mB.bottom();
}


void testCollisionPB(Paddle& mPaddle, Ball& mBall);

void testBallsCollision(Ball& mBall, Ball& secBall);


void testCollisionBB(Brick& mBrick, Ball& mBall, Paddle& mPaddle, RenderWindow& window);


bool IsTaken(Bonus* bonus, Paddle* paddle, Time gameTime);