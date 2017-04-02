#include <SFML\Graphics.hpp>
#include "Vector2.h"
#include "logic.h"

extern int BALL_PLAYER;
extern int PREVIOUS_BALL_PLAYER;
extern int CURRENT_PLAYER;

std::string getDirection(const Vector2& v)
{
	Vector2 right(1, 0);
	double cos = (right * v) / (right.len() * v.len());
	if (cos >= 0.5 && cos <= 1)
		return "right";
	if (cos >= -1 && cos <= -0.5)
		return "left";
	if (cos > -0.5 && cos < 0.5 && v.y > 0)
		return "up";
	return "down";	
}

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

bool Player::in_zone()
{
	if ((pos.x < zone_end.x && pos.x > zone_begin.x) && (pos.y < zone_end.y && pos.y > zone_begin.y))
		return true;
	return false;
}

bool Ball::in_zone(struct Player hero)
{
	if ((pos.x < hero.zone_end.x && pos.x > hero.zone_begin.x) && (pos.y < hero.zone_end.y && pos.y > hero.zone_begin.y))
		return true;
	return false;
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

	if (velocity.len() != 0)
		texture.loadFromFile("player" + std::to_string(teamID) + "_" + getDirection(velocity) + "_" + std::to_string((int)currentFrame) + ".png");

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

	bool RUN_TO_BALL = false;
	for (auto& hero : opponentPlayers)
	{
		if (!hero.in_zone())
		{
			hero.velocity = V * ((hero.zone_end + hero.zone_begin) / 2 - hero.pos).norm();
		}
		if (hero.in_zone())
		{
			if (ball.in_zone(hero) && !RUN_TO_BALL)
			{
				hero.velocity = V * (ball.pos - hero.pos).norm();
				RUN_TO_BALL = true;
				if (abs((ball.pos - hero.pos).x) < V / 2 && abs((ball.pos - hero.pos).y) < V / 2)
					hero.velocity = ball.pos - hero.pos;
			}
			else if (RUN_TO_BALL)
			{
				hero.velocity = V * (ball.pos - hero.pos).norm();
				if (abs((ball.pos - hero.pos).x) < 2 * V && abs((ball.pos - hero.pos).y) <  2 * V)
					hero.velocity = Vector2(0, 0);
			}
			else
			{
				hero.velocity = V * ((hero.zone_end + hero.zone_begin) / 2 - hero.pos).norm();
				if (abs(((hero.zone_end + hero.zone_begin) / 2 - hero.pos).x) < V  && abs(((hero.zone_end + hero.zone_begin) / 2 - hero.pos).y) < V)
					hero.velocity = (hero.zone_end + hero.zone_begin) / 2 - hero.pos;
			}
		}
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
