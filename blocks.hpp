#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "ball.hpp"

using namespace std;
using namespace sf;

constexpr int infinite_block{ 4 }, max_lives{ 3 };
constexpr float blockWidth{ 60.f }, blockHeight{ 20.f };
constexpr int countBlocksX{ 11 }, countBlocksY{ 4 };



struct Rectangle
{
    RectangleShape shape;
    float x() { return shape.getPosition().x; }
    float y() { return shape.getPosition().y; }
    float left() { return x() - shape.getSize().x / 2.f; }
    float right() { return x() + shape.getSize().x / 2.f; }
    float top() { return y() - shape.getSize().y / 2.f; }
    float bottom() { return y() + shape.getSize().y / 2.f; }
};


struct Brick : public Rectangle
{
    
    bool destroyed{ false};
    void destroy(){ destroyed = true; };
    char bonus;
    int hardness;
    int tmp;
    Brick(float mX, float mY);
};



struct MovingBlock : public Brick {

    MovingBlock(float _x, float _y);
    void update();
    Vector2f velocity{ 5.f, 0.f };
};

