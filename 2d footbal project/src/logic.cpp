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
	if (cos > sqrt(2) / 2 && cos <= 1)
		return "_right_";
	if (cos >= -1 && cos < -sqrt(2) / 2)
		return "_left_";
	if (cos > -sqrt(2) / 2 && cos <= sqrt(2) / 2 && v.x > 0)
		return "_up_";
	return "_down_";	
}

void Ball::update(float dt)
{
	size = (20 * pos.y / (WINDOW_SIZE.y + 1)) * 0.02f + 0.3f;
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

void Team::setPositions(Ball& ball)
{
	bool RUN_TO_BALL = false;
	for (auto& hero : players)
	{
		if (!hero.in_zone() && !hero.withBall)
		{
			hero.velocity = V * ((hero.zone_end + hero.zone_begin) / 2 - hero.pos).norm();
		}
		else
		{
			if (ball.in_zone(hero) && !RUN_TO_BALL && !hero.withBall)
			{
				hero.velocity = V * (ball.pos - hero.pos).norm();
				RUN_TO_BALL = true;
				if (abs((ball.pos - hero.pos).x) < V / 2 && abs((ball.pos - hero.pos).y) < V / 2)
					hero.velocity = Vector2(0, 0);
			}
			else if (ball.in_zone(hero) && RUN_TO_BALL && !hero.withBall)
			{
				hero.velocity = V * (ball.pos - hero.pos).norm();
				if (abs((ball.pos - hero.pos).x) < 2 * V && abs((ball.pos - hero.pos).y) <  2 * V)
					hero.velocity = Vector2(0, 0);
			}
			else if (!hero.withBall)
			{
				hero.velocity = V * ((hero.zone_end + hero.zone_begin) / 2 - hero.pos).norm();
				if (abs(((hero.zone_end + hero.zone_begin) / 2 - hero.pos).x) < V  && abs(((hero.zone_end + hero.zone_begin) / 2 - hero.pos).y) < V)
					hero.velocity = hero.velocity = Vector2(0, 0);
			}
		}
	}
}

void Player::stopPlayer()
{
	acceleration = Vector2(0, 0);
	velocity = Vector2(0, 0);
}

void Player::update(float dt)
{
	size = (20 * pos.y / (WINDOW_SIZE.y + 1)) * 0.02f + 0.3f;
	radius = size *  Vector2(texture.getSize().x / 2, texture.getSize().y / 2);
	velocity += acceleration * dt;
	if (velocity.len() > MAX_PLAYER_VELOCITY)
		velocity = MAX_PLAYER_VELOCITY * velocity.norm();
	pos += velocity * dt;
	
	currentFrame += velocity.len()/10 * dt;
	Vector2 v = velocity.norm();
	if (currentFrame > 4)
		currentFrame -= 4;

	texture.loadFromFile("player" + std::to_string(teamID) + "_stop.png");

	if (velocity.len() != 0)
		texture.loadFromFile("player" + std::to_string(teamID) + getDirection(velocity) + std::to_string((int)currentFrame) + ".png");

}

void Camera::setPosition()
{
	if (pos.x <= 400)
		pos.x = 400;
	if (pos.x >= 880)
		pos.x = 880;
}

void Team::createTeam(const char teamID)
{
	for (int i = 0; i < PLAYERS_AMOUNT; i++)
	{
		Player player;
		player.withBall = false;
		player.stopPlayer();
		player.teamID = teamID;
		player.currentFrame = 0;
		player.texture.loadFromFile("player" + std::to_string(teamID) + "_stop.png");
		player.texture.setSmooth(true);
		player.pos = Vector2(i * 100 + 50, i * 100 + 50);

		players.push_back(player);
	}
}

void Map::update(float dt)
{
	for (auto& hero : myTeam.players)
	{
		if (hero.pos.x + hero.radius.x >= size.x && hero.velocity.x > 0)
		{
			hero.stopPlayer();
		}
		if (hero.pos.x - hero.radius.x <= 0 && hero.velocity.x < 0)
		{
			hero.stopPlayer();
		}
		if (hero.pos.y - hero.radius.y <= 0 && hero.velocity.y < 0)
		{
			hero.stopPlayer();
		}
		if (hero.pos.y + hero.radius.y >= size.y && hero.velocity.y > 0)
		{
			hero.stopPlayer();
		}
		hero.update(dt);
	}

	opponentTeam.setPositions(ball);
	myTeam.setPositions(ball);

	for (auto& hero : opponentTeam.players)
	{
		hero.update(dt);
	}

	for (auto& hero : myTeam.players)
	{
		hero.update(dt);
	}

	for (int i = 0; (i < myTeam.players.size()) && (BALL_PLAYER < 0); i++)
	{
		if (i != PREVIOUS_BALL_PLAYER)
		{
			Vector2 d = ball.pos - myTeam.players[i].pos - Vector2(0, myTeam.players[i].radius.y);
			if (d.len() < ball.radius.x + myTeam.players[i].radius.x)
			{
				BALL_PLAYER = i;
				myTeam.players[i].withBall = true;
				CURRENT_PLAYER = BALL_PLAYER;
				PREVIOUS_BALL_PLAYER = -1;
			}
			else
			{
				BALL_PLAYER = -1;
				myTeam.players[i].withBall = false;
			}
		}
	}

	if (BALL_PLAYER >= 0)
	{
		ball.pos = myTeam.players[BALL_PLAYER].pos + Vector2(5, myTeam.players[BALL_PLAYER].radius.y);
		ball.velocity = myTeam.players[BALL_PLAYER].velocity;
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
	camera.setPosition();
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
