#include "game.hpp"

vector<Block> CreateBlocks();
Bonus* CreateBonuses(Block block, Carriage* carriage, Ball* ball, RenderWindow* window, Player* player, Interaction* interaction);
void FillText(Text* text, string _s, int _size, Color _color, float _x, float _y);

int main() {
    srand(time(0));
    Clock gameClock;
    ResultTable table{ 0, 60 };
    RenderWindow window{ {windowWidth, windowHeight}, "Arkanoid" };
    Ball ball{ windowWidth / 2, windowHeight / 2 };
    Carriage carriage{ windowWidth / 2, windowHeight - 50 };
    Interaction interaction;
    vector<Block> blocks = CreateBlocks();
    vector <Bonus*> bonuses;
    vector <Bonus*> activeBonuses;
    Player player;
    bool onlyUnbreakable = true;
    Bonus bonus;
    string score;
    bool isSafe = false;
    Font font;
    font.loadFromFile("arial.ttf");

    Text text("", font);
    FillText(&text, "Score:", 45, Color::White, windowWidth / 15.f, 3.f);
    Text resultText("0", font);
    FillText(&resultText, "", 45, Color::White, windowWidth / 4.f, 5.f);
    Text finishText("", font);
    FillText(&finishText, "GAME OVER", 100, Color::White, windowWidth / 9.f, windowHeight / 3.f);
    Text endText("", font);
    FillText(&endText, "", 50, Color::White, windowWidth / 3.5, windowHeight / 2.f);
    Text spaceText("", font);
    FillText(&spaceText, "Press Escape to close the window", 40, Color::Yellow, windowWidth / 7.f, 3.f * windowHeight / 4.f);
    Text escapeText("", font);
    FillText(&escapeText, "Press Escape to finish", 30, { 169, 169, 169, 255 }, windowWidth / 2.f, 15.f);


    while (true) {
        window.clear(Color::Black);
        window.draw(table.shape);
        if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) { break; };
        window.draw(ball.shape);
        window.draw(carriage.shape);
        window.draw(text);
        window.draw(escapeText);
        ball.update(ball.GetBallVelocityX(), ball.GetBallVelocityY());
        carriage.update();
        interaction.solveCollision(ball, carriage);

        for (int i = 0; i < blocks.size(); i++) {
            Type type = interaction.solveCollision(ball, blocks[i]);
            if (type != null && type != unbreakable) {
                player.SetScore(player.GetScore() + 1);
            }
            if (type == withBonus) {
                Bonus* tmpBonus = CreateBonuses(blocks[i], &carriage, &ball, &window, &player, &interaction);
                bonuses.push_back(tmpBonus);
            }
            if (type == speedUp) {
                ball.SetVelocityX(ballVelocity);
                ball.SetVelocityY(ballVelocity + 0.2);
            }
            if (blocks[i].GetType() != unbreakable) {
                onlyUnbreakable = false;
            }
        }
        for (int i = 0; i < bonuses.size(); i++) {
            window.draw(bonuses.at(i)->shape);
            bonuses.at(i)->update();
            if (interaction.IsActivated(bonuses.at(i), &carriage, gameClock.getElapsedTime()) == true) {
                bonuses.at(i)->shape.setFillColor({ 0, 0, 0, 0 });
                activeBonuses.push_back(bonuses.at(i));
                bonuses.erase(bonuses.begin() + i);
                continue;
            }
            if (interaction.IsActivated(bonuses.at(i), &carriage, gameClock.getElapsedTime()) == false && bonuses.at(i)->getY() >= windowHeight) {
                bonuses.erase(bonuses.begin() + i);
            }
        }
        if (ball.bottom() >= windowHeight) {
            isSafe = false;
            for (int i = 0; i < activeBonuses.size(); i++) {
                if (activeBonuses.at(i)->GetType() == safeBottomBonus) {
                    isSafe = true;
                }
            }
            if (!isSafe) {
                player.SetScore(player.GetScore() - 1);
            }
        }
        for (int i = 0; i < activeBonuses.size(); i++) {
            activeBonuses.at(i)->BonusCheck(&carriage, &ball, &window, &player, gameClock.getElapsedTime());
            if (activeBonuses.at(i)->IsActive() == false) {
                activeBonuses.erase(activeBonuses.begin() + i);
            }
        }
        blocks.erase(remove_if(begin(blocks), end(blocks), [](const Block& _block) {return _block.destroyed;}), end(blocks));
        for (int i = 0; i < blocks.size(); i++) {
            window.draw(blocks[i].shape);
        }
        score = to_string(player.GetScore());
        resultText.setString(score);
        window.draw(text);
        window.draw(resultText);
        if (onlyUnbreakable == true) {
            ball.SetVelocityX(0.f);
            ball.SetVelocityY(0.f);
            carriage.SetVelocityX(0.f);
            carriage.SetVelocityY(0.f);

            window.draw(finishText);
            if (player.GetScore() > 0) {
                endText.setString("You've won");
            }
            if (player.GetScore() < 0) {
                endText.setString("You've lose");
            }
            else {
                endText.setString("Your score: 0");
            }
            window.draw(endText);
            window.draw(spaceText);
            if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) break;
        }
        window.display();
    }
    return 0;
}

vector<Block> CreateBlocks() {
    vector<Block> blocks;
    for (int i = 0; i < countBlocksX; i++) {
        for (int j = 0; j < countBlockY; j++) {
            int tmp = rand() % 4;
            if (tmp == 0) {
                Block blockTmp{ (i + 1) * (blockWidth + 3) + 22, 50 + (j + 2) * (blockHeight + 3) };
                blocks.push_back(blockTmp);
            }
            if (tmp == 1) {
                UnbreakableBlock blockTmp{ (i + 1) * (blockWidth + 3) + 22, 50 + (j + 2) * (blockHeight + 3) };
                blocks.push_back(blockTmp);
            }
            if (tmp == 2) {
                BlockWithBonus blockTmp{ (i + 1) * (blockWidth + 3) + 22, 50 + (j + 2) * (blockHeight + 3) };
                blocks.push_back(blockTmp);
            }
            if (tmp == 3) {
                SpeedUpBlock blockTmp{ (i + 1) * (blockWidth + 3) + 22, 50 + (j + 2) * (blockHeight + 3) };
                blocks.push_back(blockTmp);
            }
        }
    }
    return blocks;
};

Bonus* CreateBonuses(Block block, Carriage* carriage, Ball* ball, RenderWindow* window, Player* player, Interaction* interaction) {
    vector <Bonus*> bonuses;
    int tmp = rand() % 6;
    if (tmp == 0) {
        Bonus* bonus = new SizeIncreaseBonus{ block.getX(), block.getY(), carriage };
        bonuses.push_back(bonus);
        return bonus;
    }
    if (tmp == 1) {
        Bonus* bonus = new SpeedUpBonus{ block.getX(), block.getY(), ball };
        bonuses.push_back(bonus);
        return bonus;
    }
    if (tmp == 2) {
        Bonus* bonus = new SafeBottomBonus{ block.getX(), block.getY(), window, ball };
        bonuses.push_back(bonus);
        return bonus;
    }
    if (tmp == 3) {
        Bonus* bonus = new StickCarriageBonus{ block.getX(), block.getY(), carriage, ball };
        bonuses.push_back(bonus);
        return bonus;
    }
    if (tmp == 4) {
        Bonus* bonus = new ChangeWayBonus{ block.getX(), block.getY(), ball };
        bonuses.push_back(bonus);
        return bonus;
    }
    if (tmp == 5) {
        Bonus* bonus = new NewBlockBonus{ block.getX(), block.getY(),ball, window, player, interaction };
        bonuses.push_back(bonus);
        return bonus;
    }
}

void FillText(Text* text, string _s, int _size, Color _color, float _x, float _y) {
    text->setCharacterSize(_size);
    text->setStyle(Text::Bold);
    text->setColor(_color);
    text->setPosition(_x, _y);
    text->setString(_s);
    return;
}