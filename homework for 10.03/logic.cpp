#include "logic.h"

/*
fixit: вам не нужна эта глобальная переменная. можно просто вызвать bullets.size()
*/

extern int currentBul;

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
			/* вы где-то в коде удаляли через swap с последним. это эффективнее */
			bullets.erase(bullets.begin() + i);
			currentBul--;
		}
		else
		{
			b.update(dt);
		}
		i++;
	}
	
	/* не уверен, что проверка на строгое равенство отработает корректно ... если за один шаг dt герой перескочит пиксель 0,
	то герой спокойно себе вылетит с поля*/
	if (hero.pos.x == size.x && hero.velocity.x > 0)
		hero.velocity.x = 0;
	if (hero.pos.x == 0 && hero.velocity.x < 0)
		hero.velocity.x = 0;
	if (hero.pos.y == 0 && hero.velocity.y < 0)
		hero.velocity.y = 0;
	if (hero.pos.y == size.y && hero.velocity.y > 0)
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
