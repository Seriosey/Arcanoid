#pragma once
#include "mech.hpp"


vector <Bonus*> bonuses;

int score{ 0 };

void testBallsCollision(Ball& mBall, Ball& secBall) {
    if (!isIntersecting(mBall, secBall)) return;//change direction of ball, if it's intersecting paddle

    float overlapLeft{ mBall.right() - secBall.left() };//check relative spatial location of ball and brick
    float overlapRight{ secBall.right() - mBall.left() };
    float overlapTop{ mBall.bottom() - secBall.top() };
    float overlapBottom{ secBall.bottom() - mBall.top() };

    bool ballFromLeft(abs(overlapLeft) < abs(overlapRight));
    bool ballFromTop(abs(overlapTop) < abs(overlapBottom));

    float minOverlapX{ ballFromLeft ? overlapLeft : overlapRight };
    float minOverlapY{ ballFromTop ? overlapTop : overlapBottom };

    if (abs(minOverlapX) < abs(minOverlapY)) {
        mBall.velocity.x = ballFromLeft ? -ballVelocity : ballVelocity;
        secBall.velocity.x = ballFromLeft ? ballVelocity : -ballVelocity;
    }
    else {
        mBall.velocity.y = ballFromTop ? -ballVelocity : ballVelocity;
        secBall.velocity.y = ballFromTop ? ballVelocity : -ballVelocity;
    }
}

void testCollisionPB(Paddle& mPaddle, Ball& mBall)
{
    if (!isIntersecting(mPaddle, mBall)) return;//change direction of ball, if it's intersecting paddle

    mBall.velocity.y = -ballVelocity;
    if (mBall.x() < mPaddle.x())
        mBall.velocity.x = -ballVelocity;
    else
        mBall.velocity.x = ballVelocity;
}

void testCollisionBB(Brick& mBrick, Ball& mBall, Paddle& mPaddle, RenderWindow& window)
{
    if (!isIntersecting(mBrick, mBall)) return;
    mBrick.hardness -= 1;
    if (mBrick.hardness == 0) 
    mBrick.destroy();
    score += 1;
    float mx = mBrick.x();
    float my = mBrick.y();

    switch (mBrick.bonus) {
    case 's': {
        ballVelocity *= 1.02;
        //mBall.setVelocityX(30);
        //mBall.setVelocityY(30);
        //mBall.IncreaseVelocity(1.4);
        break;}
    case 'l': {
        Bonus* tmpBonus = CreateBonuses(mx, my, &mPaddle, &mBall, &window);//create bonus item if ball collided with brick with bonus
        bonuses.push_back(tmpBonus);

        break;}
    case 'u':
        mBrick.destroyed = false;
        break;
    }

    float overlapLeft{ mBall.right() - mBrick.left() };//check relative spatial location of ball and brick
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
    if (!isIntersecting(*bonus, *paddle)) {
        return false;
    }
    else {
        bonus->GetBonusClock().restart().asSeconds();//turning bonus-life timer on
        bonus->SetActivity(true);
        return true;
    }
}