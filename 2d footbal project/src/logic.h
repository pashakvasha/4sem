#pragma once
#include <SFML\Graphics.hpp>
#include "Vector2.h"

const Vector2 MAP_SIZE = Vector2(800, 600);
const int MAX_BALL_VELOCITY = 500;
const int MAX_BALL_ACCELERATION = 300;
const int MAX_PLAYER_ACCELERATION = 500;
const int MAX_PLAYER_VELOCITY = 100;
const int PLAYERS_AMOUNT = 4;
const float V = 70;

struct Ball
{
	Vector2 pos;
	sf::Texture texture;
	Vector2 velocity;
	Vector2 acceleration;
	Vector2 radius;
	float size;
	float angle;
	void update(float dt);
};

struct Player
{
	Vector2 pos;
	Vector2 velocity;
	Vector2 radius;
	char teamID;
	float currentFrame;
	sf::Texture texture;
	float size;
	void update(float dt);
};

struct Map
{
	Ball ball;
	std::vector<Player> players;
	std::vector<Player> opponentPlayers;
	Vector2 size;
	void update(float dt);

};

void createTeams(Map& map);
void createBall(Ball& ball);