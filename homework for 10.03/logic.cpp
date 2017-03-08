#include "logic.h"

void Bullet::update(float dt)
{
	pos += velocity * dt;
}

void Hero::update(float dt)
{
	pos += velocity * dt;
}

void Map::update(float dt)
{
	int i = 0;
	for (auto& b : bullets)
	{
		if ((b.pos.x > size.x) || (b.pos.y > size.y)
			|| (b.pos.x < 0) || (b.pos.y < 0))
		{
			//deletion using swap sometimes works incorrectly
			//std::swap(b, bullets.back());
			//bullets.pop_back();
			bullets.erase(bullets.begin() + i);
		}
		else
		{
			b.update(dt);
		}
		i++;
	}
	if (hero.pos.x + hero.radius.x / 2 >= size.x && hero.velocity.x > 0)
		hero.velocity.x = 0;
	if (hero.pos.x - hero.radius.x / 2 <= 0 && hero.velocity.x < 0)
		hero.velocity.x = 0;
	if (hero.pos.y - hero.radius.y / 4 <= 0 && hero.velocity.y < 0)
		hero.velocity.y = 0;
	if (hero.pos.y + hero.radius.y / 4 >= size.y && hero.velocity.y > 0)
		hero.velocity.y = 0;

	hero.update(dt);
}

sf::Vector2f getRotated(float degree, sf::Vector2f vector)
{
	sf::Vector2f res;
	float x_ = vector.x, y_ = vector.y;
	float angle = degree / 180 * Pi;
	res.x = x_ * cos(angle) - y_ * sin(angle);
	res.y = y_ * cos(angle) + x_ * sin(angle);
	return res;
}