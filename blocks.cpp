#include "blocks.hpp"

float moveBlockVelocity{ 4.f };

Brick::Brick(float mX, float mY)
{
	hardness = 3;
	bonus = 'd';
	shape.setPosition(mX, mY);
	shape.setSize({ blockWidth, blockHeight });
	shape.setFillColor(Color::Yellow);
	shape.setOrigin(blockWidth / 2.f, blockHeight / 2.f);
	tmp = rand() % 6;
	if (tmp == 0) { shape.setFillColor(Color::White); bonus = 's'; }
	if (tmp == 1) { shape.setFillColor(Color::Magenta); bonus = 'l'; }
	if (tmp == 2) { shape.setFillColor(Color::Red);bonus = 'u'; }
}

MovingBlock::MovingBlock(float _x, float _y) : Brick(_x, _y) {
	shape.setFillColor(Color::Green);
	//SetType(movingBlock);
	velocity.x = moveBlockVelocity;
	velocity.y = 0;

}
void MovingBlock::update() {
	shape.move({ moveBlockVelocity, 0 });
	if (left() <= 0) {
		shape.move({ moveBlockVelocity, 0 });
		velocity.x = moveBlockVelocity;
	}
	else if (right() >= windowWidth) {
		shape.move({ -moveBlockVelocity, 0 });
		velocity.x = -2 * moveBlockVelocity;
	}
	shape.move(velocity);
}