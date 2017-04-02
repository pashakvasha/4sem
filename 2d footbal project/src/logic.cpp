#include <SFML\Graphics.hpp>
#include "Vector2.h"
#include "logic.h"

extern int BALL_PLAYER;
extern int PREVIOUS_BALL_PLAYER;
extern int CURRENT_PLAYER;

void Ball::update(float dt)
{
	size = (20 * pos.y / (MAP_SIZE.y + 1)) * 0.02f + 0.3f;
	radius = 0.1 * size * Vector2(texture.getSize().x / 2, texture.getSize().y / 2);
	pos += velocity * dt;
	angle += 50 * velocity.len() / radius.x * dt;
	if (velocity * acceleration < 0)
		velocity += acceleration * dt;
	else
		velocity = Vector2(0, 0);
}

void Player::update(float dt)
{
	size = (20 * pos.y / (MAP_SIZE.y + 1)) * 0.02f + 0.3f;
	radius = size *  Vector2(texture.getSize().x / 2, texture.getSize().y / 2);

	pos += velocity * dt;

	currentFrame += 10 * dt;
	Vector2 v = velocity.norm();
	if (currentFrame > 4)
		currentFrame -= 4;
	texture.loadFromFile("player" + std::to_string(teamID) + "_stop.png");

	if ( v.x == 1 && v.y == 0)
	{
		texture.loadFromFile("player" + std::to_string(teamID) + "_right_" + std::to_string((int)currentFrame / 2) + ".png");
	}

	if (v.x == -1 && v.y == 0)
	{
		texture.loadFromFile("player" + std::to_string(teamID) + "_left_" + std::to_string((int)currentFrame / 2) + ".png");
	}

	Vector2 d = Vector2(1, -1).norm();
	if (v.x == d.x && v.y == d.y)
	{
		texture.loadFromFile("player" + std::to_string(teamID) + "_up_" + std::to_string((int)currentFrame) + ".png");
	}

	d = Vector2(-1, 1).norm();
	if (v.x == d.x && v.y == d.y)
	{
		texture.loadFromFile("player" + std::to_string(teamID) + "_down_" + std::to_string((int)currentFrame) + ".png");
	}
}

void Map::update(float dt)
{
	for (auto& hero : players)
	{
		if (hero.pos.x + hero.radius.x >= size.x && hero.velocity.x > 0)
			hero.velocity = Vector2(0, 0);
		if (hero.pos.x - hero.radius.x <= 0 && hero.velocity.x < 0)
			hero.velocity = Vector2(0, 0);
		if (hero.pos.y - hero.radius.y <= 0 && hero.velocity.y < 0)
			hero.velocity = Vector2(0, 0);
		if (hero.pos.y + hero.radius.y >= size.y && hero.velocity.y > 0)
			hero.velocity = Vector2(0, 0);
		hero.update(dt);
	}

	for (auto& hero : opponentPlayers)
	{
		hero.update(dt);
	}

	for (int i = 0; (i < players.size()) && (BALL_PLAYER < 0); i++)
	{
		if (i != PREVIOUS_BALL_PLAYER)
		{
			Vector2 d = ball.pos - players[i].pos - Vector2(0, players[i].radius.y);
			if (d.len() < ball.radius.x + players[i].radius.x)
			{
				BALL_PLAYER = i;
				CURRENT_PLAYER = BALL_PLAYER;
				PREVIOUS_BALL_PLAYER = -1;
			}
			else
			{
				BALL_PLAYER = -1;
			}
		}
	}

	if (BALL_PLAYER >= 0)
	{
		ball.pos = players[BALL_PLAYER].pos + Vector2(5, players[BALL_PLAYER].radius.y);
		ball.velocity = players[BALL_PLAYER].velocity;
	}

	if (ball.pos.x + ball.radius.x >= size.x && ball.velocity.x > 0)
		ball.velocity = Vector2(0, 0);
	if (ball.pos.x - ball.radius.x <= 0 && ball.velocity.x < 0)
		ball.velocity = Vector2(0, 0);
	if (ball.pos.y - ball.radius.y <= 0 && ball.velocity.y < 0)
		ball.velocity = Vector2(0, 0);
	if (ball.pos.y + ball.radius.y >= size.y && ball.velocity.y > 0)
		ball.velocity = Vector2(0, 0);

	ball.update(dt);
}

void createTeams(Map& map)
{
	for (size_t i = 0; i < PLAYERS_AMOUNT; i++)
	{
		Player player;
		Player opponent;

		player.teamID = 1;
		player.currentFrame = 0;
		player.texture.loadFromFile("player1_stop.png");
		player.texture.setSmooth(true);
		player.pos = Vector2(i * 100 + 50, i * 100 + 50);

		opponent.texture.loadFromFile("player2_stop.png");
		opponent.texture.setSmooth(true);
		opponent.teamID = 2;
		opponent.currentFrame = 0;
		opponent.pos = Vector2(800 - i * 100 - 50, i * 100 + 50);

		map.opponentPlayers.push_back(opponent);
		map.players.push_back(player);
	}
}

void createBall(Ball& ball)
{
	ball.texture.loadFromFile("ball.png");
	ball.texture.setSmooth(true);
	ball.pos = Vector2(500, 500);
	ball.angle = 0;
	ball.velocity = Vector2(0, 0);
	ball.acceleration = Vector2(0, 0);
}
