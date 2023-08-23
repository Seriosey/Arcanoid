#include "ball.hpp"


class Game {
public:
	Ball ball{ window_width / 2, window_height / 2, Color::Red, 0 };
	Paddle paddle{ window_width / 2, window_height - paddle_offset };
	RectangleShape line{ Vector2f(window_width, 5) };
	Font arial;
	Text text;
	RenderWindow window{ {window_width, window_height}, "Arkanoid" };

	bool run();
	vector<unique_ptr<Block>> initBlocks();
	vector<unique_ptr<Bonus>> bonus_vec;
	vector<unique_ptr<Block>> blocks_vec;
	vector <unique_ptr<Ball>> balls_vec;
	vector < unique_ptr<MovingBlock>> mov_block_vec;

};