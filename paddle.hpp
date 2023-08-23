#pragma once
#include "blocks.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;
using namespace sf;

extern float paddleWidth;
constexpr float  paddleHeight{ 20.f }, paddleVelocity{ 10.f };
constexpr int paddle_offset{ 50 };

struct Paddle : public Rectangle
{
    Paddle(float mX, float mY);
    Vector2f velocity;
    void update();
    void SetVelocity(float vel) { velocity.x = vel; };
    float GetVelocity() { return velocity.x; };
};