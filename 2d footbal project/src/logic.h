#pragma once
#include <SFML\Graphics.hpp>
#include "Vector2.h"

const Vector2 WINDOW_SIZE = Vector2(800, 600);
const int MAX_BALL_VELOCITY = 600;
const int MAX_BALL_ACCELERATION = 300;
const int MAX_PLAYER_ACCELERATION = 10500;
const int MAX_PLAYER_VELOCITY = 150;
const int PLAYERS_AMOUNT = 6;
const float V = 70;

struct Ball
{
	Vector2 pos;
	Vector2 velocity;
	Vector2 acceleration;
	Vector2 radius;
	bool in_zone(struct Player hero);
	float size;
	float angle;
	sf::Texture texture;
	void moveBall(const Vector2 & direction); // direction must be a unit vector
	void update(float dt);
};

struct Player
{
	Vector2 pos;
	Vector2 velocity;
	Vector2 radius;
	Vector2 acceleration;
	Vector2 zone_begin;
	Vector2 zone_end;
	bool withBall;
	bool currentPlayer;
	bool in_zone();
	float currentFrame;
	float size;
	char teamID;
	sf::Texture texture;
	void movePlayer(const Vector2 & direction); // direction must be a unit vector
	void acceleratePlayer();
	void stopPlayer();
	void update(float dt);
};

struct Team
{
	std::vector<Player> players;
	bool runToBall;
	void setPositions(Ball& ball);
	void createTeam(const char teamID, const Vector2& fieldSize);
};

struct Camera
{
	Vector2 pos;
	void setPosition();
};

struct Map
{
	Ball ball;
	Camera camera;
	Team myTeam;
	Team opponentTeam;
	Vector2 size;
	void update(float dt);

};


void createBall(Ball& ball);