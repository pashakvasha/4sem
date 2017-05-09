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

struct Line
{
	Vector2 begin;
	Vector2 end;
	bool isPointOnLine(const Vector2& point);
};

enum class ActionType
{
	Run, Pass, ChangeCurrentPlayer, Accelerate
};

struct Action
{
	int playerID;
	ActionType type;
	Vector2 dir;
};

struct PlayerTextures
{
	sf::Texture stop;
	std::vector<sf::Texture> left;
	std::vector<sf::Texture> right;
	std::vector<sf::Texture> up;
	std::vector<sf::Texture> down;
};

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

	void createBall();
	void checkFieldBoundary(const Vector2& boundary);
	void moveBall(const Vector2 & direction); // direction must be a unit vector
	void update(float dt);
	void drawBall(sf::RenderWindow& window);
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
	sf::Texture texture;
	
	void setTexture(const PlayerTextures& textures);
	void setStartPosition();
	void movePlayer(const Vector2 & direction); // direction must be a unit vector
	void acceleratePlayer();
	void stopPlayer();
	//IT IS ESSENTIAL to write function checkFieldBoundary
	void update(float dt);
};

struct Team
{
	std::vector<Player> players;
	bool runToBall;
	char currentPlayer;
	PlayerTextures textures;
	char teamID;
	int goalAmount = 0;
	sf::Texture pointerTexture;

	void downloadTextures();
	void update(Ball& ball, float dt);
	void createTeam(const char& ID, const Vector2& fieldSize);
	void drawTeam(sf::RenderWindow& window);
};

struct Camera
{
	sf::View view;
	Vector2 pos;

	void setPosition(const Vector2& position);
};

struct Map
{
	bool withBall;
	bool goal;
	float time = 0;
	Ball ball;
	Camera camera;
	Team myTeam;
	Team opponentTeam;
	Vector2 size;
	Line myGate;
	Line opponentGate;
	sf::Texture texture;
	sf::Text scoreboard;
	sf::Font font;

	void resetGame();
	void hit();
	void changeCurrentPlayer();
	void passToPlayer();
	void createField();
	void update(float dt);
	void drawField(sf::RenderWindow& window);

};
