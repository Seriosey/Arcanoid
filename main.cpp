#pragma once
#include <SFML/Graphics.hpp>
#include<iostream>
#include<vector>
#include<string>
#include "mech.hpp"

using namespace std;
using namespace sf;


int main()
{
    Ball ball{ windowWidth / 2, windowHeight / 2 };
    
    Paddle paddle{ windowWidth / 2, windowHeight - 50 };
    vector<Brick> bricks;
    vector <Bonus*> activeBonuses;
    srand(time(0));
    Clock gameClock;


    for (int iX{ 0 }; iX < countBlocksX; ++iX)//bricks grid initialization
        for (int iY{ 0 }; iY < countBlocksY; ++iY)
            bricks.emplace_back(
                (iX + 1) * (blockWidth + 3) + 22, (iY + 2) * (blockHeight + 3));

    RenderWindow window(VideoMode(800, 600), "Arkanoid ");
    window.setFramerateLimit(60);

    while (true)
    {
        window.clear(Color::Black);

        if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) {
            cout << "Score is " << score << endl; break;
        }

        //ball.setRadius(30);

        ball.update();
        paddle.update();
        testCollisionPB(paddle, ball);//paddle - ball collision check
        //testBallsCollision(second_ball, ball);
        for (auto& brick : bricks) testCollisionBB(brick, ball, paddle, window);//brick - ball collision check
        //for (auto& brick : bricks) testCollisionBB(brick, second_ball, paddle, window);
        bricks.erase(remove_if(begin(bricks), end(bricks),//delete destroyed bricks
           [](const Brick& mBrick)
           {
                return mBrick.destroyed;
           }),
            end(bricks));

        window.draw(ball.shape);
        window.draw(paddle.shape);

        for (auto& brick : bricks) window.draw(brick.shape);

        for (int i = 0; i < bonuses.size(); i++) {
            window.draw(bonuses.at(i)->shape);
            bonuses.at(i)->update();
            if (IsTaken(bonuses.at(i) , &paddle, gameClock.getElapsedTime()) == true) {//check if bonus is taken be player
                bonuses.at(i)->shape.setFillColor({ 0, 0, 0, 0 });
                activeBonuses.push_back(bonuses.at(i));//add taken bonus to active bonuses
                bonuses.erase(bonuses.begin() + i);
                continue;
            }
            if (IsTaken(bonuses.at(i), &paddle, gameClock.getElapsedTime()) == false && bonuses.at(i)->y() >= windowHeight) {
                bonuses.erase(bonuses.begin() + i); //if bonus falled out of the window, delete it
            }
        }
        if (ball.bottom() >= windowHeight) {
            for (int i = 0; i < activeBonuses.size(); i++) {
                if (activeBonuses.at(i)->GetType() == "safeBottomBonus") {//if safe-bottom is active, cancel score decreasing
                    score += 1;
                }
            }
        }
        for (int i = 0; i < activeBonuses.size(); i++) {
            activeBonuses.at(i)->BonusCheck(&paddle, &ball, &window, gameClock.getElapsedTime());
            if (activeBonuses.at(i)->IsActive() == false) {//delete deactivated bonuses
                activeBonuses.erase(activeBonuses.begin() + i);
            }
        }
        window.display();
    }

    return 0;
}