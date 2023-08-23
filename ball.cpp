#include "ball.hpp"


float ballVelocity{ 8.f };

Ball::Ball(float mX, float mY)
{
    
    shape.setPosition(mX, mY);
    shape.setRadius(70);
    shape.setFillColor(Color::Blue);
    shape.setOrigin(ballRadius, ballRadius);
}

void Ball::setVelocity(float ballVelocity) { velocity.x = -ballVelocity; velocity.y = -ballVelocity; }

void Ball::update() {
        shape.move(velocity);

        if (left() < 0)
            velocity.x = ballVelocity;
        else if (right() > windowWidth)
            velocity.x = -ballVelocity;

        if (top() < 0)
            velocity.y = ballVelocity;
        else if (bottom() > windowHeight)
            velocity.y = -ballVelocity;
    }
   
