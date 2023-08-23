#include "game.hpp"
extern int player_score;

vector<unique_ptr<Block>> Game::initBlocks() {
	vector<unique_ptr<Block>> blocks;
	for (int iX = 0; iX < count_blocksX; ++iX)
		for (int iY = 0; iY < count_blocksY; ++iY) {
			unique_ptr<Block> block;
			BonusType bonus;
			Color block_color;

			int bonus_block = rand() % 5;
			switch (bonus_block) {
			case 1:
				bonus = BonusType::MOVING_BLOCK;
				block_color = Color::Blue;
				break;
			case 2:
				bonus = BonusType::SECOND_BALL;
				block_color = Color::Blue;
				break;
			default:
				bonus = BonusType::NONE;
				block_color = Color::Yellow;
				break;
			}
			int num_lives = rand() % (infinite_block + 2) + 1;
			switch (num_lives) {
			case infinite_block:
				block = move(make_unique<Block>((iX + 1) * (block_width + 3) + 22,
					(iY + 2) * (block_height + 3), infinite_block, Color::Red, BonusType::NONE));
				break;
			default:
				block = move(make_unique<Block>((iX + 1) * (block_width + 3) + 22,
					(iY + 2) * (block_height + 3), num_lives % max_lives + 1, block_color, bonus));
				break;
			}
			blocks.emplace_back(move(block));
		}
	return blocks;
}

bool Game::run() {
	if (!arial.loadFromFile("arial.ttf"))
		return false;
	line.setPosition(0, top_window_line);
	text.setFont(arial);
	text.setCharacterSize(20);
	text.setFillColor(Color::Red);


	Text exit_text("To exit press SPACE", arial, 20);
	exit_text.setFillColor(Color::Red);
	exit_text.setPosition({ 3 * window_width / 4, 0 });

	blocks_vec = initBlocks();
	balls_vec.emplace_back(make_unique<Ball>(ball));

	window.setFramerateLimit(60);
	while (true) {
		window.clear(Color::Black);
		if (Keyboard::isKeyPressed(Keyboard::Key::Space)) break;
		text.setString("Current score: " + to_string(player_score));

		paddle.update();

		for (auto& mov_block : mov_block_vec) {
			mov_block->update();	
			for (auto& block : blocks_vec)
				mov_block->BlocksCollision(*block);
		}
		for (int i = 0; i < static_cast<int>(mov_block_vec.size()); ++i) {
			for (int j = 0; j < static_cast<int>(mov_block_vec.size()); ++j) {
				if (i == j)
					continue;
				mov_block_vec[i]->BlocksCollision(*mov_block_vec[j]);
			}
		}

		for (auto& ball : balls_vec) {
			ball->update();

			if (ball->is_dead)
				continue;
			

			ball->checkCollision(paddle);
			for (auto& mov_block : mov_block_vec)
				ball->checkCollision(*mov_block);
			for (auto& block : blocks_vec) {
				ball->checkCollision(*block);

				if (block->num_lives == 0 && block->bonus_type != BonusType::NONE)
					bonus_vec.emplace_back(make_unique<Bonus>(block->x(), block->y(), block->bonus_type));
			}
		}
		for (int i = 0; i < static_cast<int>(balls_vec.size()); ++i) {
			for (int j = 0; j < static_cast<int>(balls_vec.size()); ++j) {
				if (i == j)
					continue;
				balls_vec[i]->checkCollision(*balls_vec[j]);
			}
		}
		for (auto& bonus : bonus_vec) {
			bonus->update();
			bonus->paddleCollision(paddle);
			if (bonus->taken) {
				if (bonus->bonus_type == BonusType::SECOND_BALL)
					balls_vec.emplace_back(make_unique<BonusBall>(rand() % window_width, window_height / 2));
				else
					mov_block_vec.emplace_back(make_unique<MovingBlock>(bonus->block_pos_x, bonus->block_pos_y));

			}
		}

		blocks_vec.erase(remove_if(begin(blocks_vec), end(blocks_vec), [](const unique_ptr<Block>& block)
			{ return (block->num_lives == 0); }), end(blocks_vec));
		bonus_vec.erase(remove_if(begin(bonus_vec), end(bonus_vec), [](const unique_ptr<Bonus>& bonus)
			{ return (bonus->lost == true || bonus->taken == true); }), end(bonus_vec));
		mov_block_vec.erase(remove_if(begin(mov_block_vec), end(mov_block_vec), [](const unique_ptr <MovingBlock>& block)
			{ return (block->num_lives == 0); }), end(mov_block_vec));
		balls_vec.erase(remove_if(begin(balls_vec), end(balls_vec), [](const unique_ptr<Ball>& ball)
			{ return (ball->is_dead); }), end(balls_vec));
		

		window.draw(line);
		window.draw(paddle.rectangle_shape);
		window.draw(text);
		window.draw(exit_text);
		for (auto& block : blocks_vec) window.draw(block->rectangle_shape);
		for (auto& mov_block : mov_block_vec) window.draw(mov_block->rectangle_shape);
		for (auto& ball : balls_vec) window.draw(ball->ball_shape);
		for (auto& bonus : bonus_vec) window.draw(bonus->rectangle_shape);

		window.display();

	}

	Text final_score("", arial, 35);
	final_score.setFillColor(Color::Red);
	final_score.setStyle(Text::Bold);

	while (true) {
		window.clear(Color::Black);

		if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) break;
		final_score.setString("Your score: " + to_string(player_score) + "\nPress ESC to exit");
		final_score.setPosition({ window_width / 2.f - 150, window_height / 2.f - 30 });
		window.draw(final_score);
		window.display();
	}

	return true;
}