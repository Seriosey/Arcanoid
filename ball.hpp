#pragma once
#include <SFML\Graphics.hpp>
#include "window.hpp"
#include <SFML/Window.hpp>
#include<iostream>

using namespace std;
using namespace sf;

constexpr float ballRadius{ 10.f };
extern float ballVelocity;
constexpr double default_velocity{ 6.f };


struct Ball
{
    CircleShape shape;
    Vector2f velocity{ -ballVelocity, -ballVelocity };

    Ball(float mX, float mY);

    void setVelocity(float ballVelocityCoeff);
    void IncreaseVelocity(float veloCoeff){ velocity.x *= veloCoeff; velocity.y *= veloCoeff; }
    void setVelocityX(float ballVelocity){ velocity.x = -ballVelocity; }
    void setVelocityY(float ballVelocity) { velocity.y = -ballVelocity; }
    float getVelocityX() { return velocity.y; }
    float getVelocityY() { return velocity.x; }

    float getVelocity() { return velocity.x;}

    float x() { return shape.getPosition().x; }
    float y() { return shape.getPosition().y; }
    float left() { return x() - shape.getRadius(); }
    float right() { return x() + shape.getRadius(); }
    float top() { return y() - shape.getRadius(); }
    float bottom() { return y() + shape.getRadius(); }

    void  update();
};

