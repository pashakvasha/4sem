#pragma once
#include <SFML\Graphics.hpp>

const int MAX_BULLET_COUNT = 10;
const int MAX_HERO_VELOCITY = 10;
const float MAX_BULLET_VELOCITY = 0.05f;
const float Pi = 3.14159f;

struct Laser {
	sf::Vector2f pos;
	float angle;
};

struct Bullet {
	sf::Vector2f pos;
	sf::Vector2f velocity;
	sf::Vector2f radius;
	float angle;
	void update(float dt);
};

struct Hero
{
	sf::Vector2f pos;
	sf::Vector2f velocity;
	sf::Vector2f radius;
	sf::Vector2f direction;
	void update(float dt);
};

struct Map
{
	sf::Vector2f size;
	Hero hero;
	Laser laser;
	std::vector<Bullet> bullets;
	void update(float dt);
};