#pragma once
#include <SFML\Graphics.hpp>
#include "Vector2.h"

const int MAX_VELOCITY = 100;
const int MIN_RADUIS = 10;

class MaterialPoint
{
public:
	void UpdatePosition(float dt);

	Vector2 position;
	Vector2 velocity;

	float mass;
};

class Ball :public MaterialPoint
{
public:
	float radius;
	int type;
};

struct Map
{
	sf::Vector2i size;
	std::vector <Ball> balls;
	void update(float dt);
};