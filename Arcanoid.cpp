/*#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

constexpr int windowWidth{ 800 }, windowHeight{ 600 };
constexpr float ballRadius{ 10.f };
float ballVelocity{ 8.f }, paddleWidth{ 60.f };
constexpr float  paddleHeight{ 20.f }, paddleVelocity{ 6.f };
constexpr float blockWidth{ 60.f }, blockHeight{ 20.f };
constexpr int countBlocksX{ 11 }, countBlocksY{ 4 };
int score{ 0 };
struct Ball
{
    CircleShape shape;
    Vector2f velocity{ -ballVelocity, -ballVelocity };
    void setVelocity(float mVelocityCoef) 
    { velocity.x = velocity.x * mVelocityCoef; velocity.y = velocity.y * mVelocityCoef;}

    Ball(float mX, float mY)
    {
        shape.setPosition(mX, mY);
        shape.setRadius(ballRadius);
        shape.setFillColor(Color::Red);
        shape.setOrigin(ballRadius, ballRadius);
       
    }

    void update()
    {
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

    float x() { return shape.getPosition().x; }
    float y() { return shape.getPosition().y; }
    float left() { return x() - shape.getRadius(); }
    float right() { return x() + shape.getRadius(); }
    float top() { return y() - shape.getRadius(); }
    float bottom() { return y() + shape.getRadius(); }
};

struct Loot
{
    CircleShape shape;
    Vector2f velocity{ 0, ballVelocity };

    Loot(float mX, float mY)
    {
        shape.setPosition(mX, mY);
        shape.setRadius(ballRadius);
        shape.setFillColor(Color::Red);
        shape.setOrigin(ballRadius, ballRadius);
    }

    void update()
    {
        shape.move(velocity);

        if (left() < 0)
            velocity.x = ballVelocity;
        else if (right() > windowWidth)
            velocity.x = -ballVelocity;

        if (top() < 0)
            velocity.y = ballVelocity;
        else if (bottom() > windowHeight)
            velocity.y = -ballVelocity;
            score -= 1;
    }

    float x() { return shape.getPosition().x; }
    float y() { return shape.getPosition().y; }
    float left() { return x() - shape.getRadius(); }
    float right() { return x() + shape.getRadius(); }
    float top() { return y() - shape.getRadius(); }
    float bottom() { return y() + shape.getRadius(); }
};

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

struct Paddle : public Rectangle
{
    Vector2f velocity;

    Paddle(float mX, float mY)
    {
        shape.setPosition(mX, mY);
        shape.setSize({ paddleWidth, paddleHeight });
        shape.setFillColor(Color::Red);
        shape.setOrigin(paddleWidth / 2.f, paddleHeight / 2.f);
    }

    void update()
    {
        shape.move(velocity);

        if (Keyboard::isKeyPressed(Keyboard::Key::Left) && left() > 0)
            velocity.x = -paddleVelocity;
        else if (Keyboard::isKeyPressed(Keyboard::Key::Right) &&
            right() < windowWidth)
            velocity.x = paddleVelocity;
        else
            velocity.x = 0;
    }
};

struct Brick : public Rectangle
{
    bool destroyed{ false };
    char bonus;
    int hardness{ 3 };

    Brick(float mX, float mY)
    {
        shape.setPosition(mX, mY);
        shape.setSize({ blockWidth, blockHeight });
        shape.setFillColor(Color::Yellow);
        shape.setOrigin(blockWidth / 2.f, blockHeight / 2.f);
        if (int r = rand() % 9 < 5) {
            if (int  s1 = rand() % (9) <= 1) { shape.setFillColor(Color::White); bonus = 's'; }
            else if ( int s2= rand() % (9) <= 1) { shape.setFillColor(Color::Blue); bonus = 'd'; }
            else if (int  s3 = rand() % (9) <= 1) { shape.setFillColor(Color::Magenta); bonus = 'l'; }
            else if (int  s4 = rand() % (9) <= 1) { shape.setFillColor(Color::Cyan); bonus = 'b'; }
            else if (int  s5 = rand() % (11) <= 1) bonus = 'u';
        }
    }
};



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



int main()
{
   // Loot loot(windowWidth / 2, windowHeight/2);
    Ball ball{ windowWidth / 2, windowHeight / 2 };
    Paddle paddle{ windowWidth / 2, windowHeight - 50 };
    vector<Brick> bricks;

    for (int iX{ 0 }; iX < countBlocksX; ++iX)
        for (int iY{ 0 }; iY < countBlocksY; ++iY)
            bricks.emplace_back(
                (iX + 1) * (blockWidth + 3) + 22, (iY + 2) * (blockHeight + 3));

    RenderWindow window( VideoMode(800, 600), "Arkanoid " );
    window.setFramerateLimit(60);

    while (true)
    {
        window.clear(Color::Black);

        if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) {
            cout << "Score is "<<score << endl; break;
        }

        ball.update();
        paddle.update();
        //loot.update();
        testCollisionPB(paddle, ball);
        for (auto& brick : bricks) testCollisionBB(brick, ball, paddle);
        bricks.erase(remove_if(begin(bricks), end(bricks),
            [](const Brick& mBrick)
            {
                return mBrick.destroyed;
            }),
            end(bricks));

        window.draw(ball.shape);
        window.draw(paddle.shape);
        
        for (auto& brick : bricks) window.draw(brick.shape);
        window.display();
    }

    return 0;
}*/