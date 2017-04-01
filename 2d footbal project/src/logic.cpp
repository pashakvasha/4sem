#include <SFML\Graphics.hpp>
#include "Vector2.h"
#include "logic.h"

extern int BALL_PLAYER;
extern int PREVIOUS_BALL_PLAYER;
extern int CURRENT_PLAYER;

void Ball::update(float dt)
{
	size = (20 * pos.y / (MAP_SIZE.y + 1)) * 0.02f + 0.3f;
	radius = Vector2(0.1 * size * texture.getSize().x / 2, 0.1 * size * texture.getSize().y / 2);
	pos += velocity * dt;
	if (velocity * acceleration < 0)
		velocity += acceleration * dt;
	else
		velocity = Vector2(0, 0);
}

void Player::update(float dt)
{
	size = (20 * pos.y / (MAP_SIZE.y + 1)) * 0.02f + 0.3f;
	radius = Vector2(size * texture.getSize().x / 2, size * texture.getSize().y / 2);
	pos += velocity * dt;
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

		player.texture.loadFromFile("player1_left_0.png");
		player.texture.setSmooth(true);

		opponent.texture.loadFromFile("player2.png");
		opponent.texture.setSmooth(true);

		player.pos = Vector2(i * 100 + 50, i * 100 + 50);
		opponent.pos = Vector2(800 - i * 100 - 50, i * 100 + 50);

		map.opponentPlayers.push_back(opponent);
		map.players.push_back(player);
	}
}

void createBall(Ball& ball)
{
	ball.texture.loadFromFile("ball.png");
	ball.texture.setSmooth(true);
	ball.pos = Vector2(400, 400);
	ball.velocity = Vector2(0, 0);
	ball.acceleration = Vector2(0, 0);
}
