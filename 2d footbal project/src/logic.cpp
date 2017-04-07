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

void Ball::moveBall(const Vector2& direction)
{
	velocity = MAX_BALL_VELOCITY * direction;
	acceleration = -MAX_BALL_ACCELERATION * velocity.norm();
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
	for (auto& player : players)
	{
		if (!player.in_zone() && !player.currentPlayer)
		{
			Vector2 directionToZoneCenter = ((player.zone_end + player.zone_begin) / 2 - player.pos).norm();
			player.movePlayer(directionToZoneCenter);
		}
		else
		{
			if (ball.in_zone(player) && !RUN_TO_BALL && !player.currentPlayer)
			{
				Vector2 directionToBall = (ball.pos - player.pos).norm();
				player.movePlayer(directionToBall);
				RUN_TO_BALL = true;
				if (abs((ball.pos - player.pos).x) < V / 2 && abs((ball.pos - player.pos).y) < V / 2)
					player.stopPlayer();
			}
			else if (ball.in_zone(player) && RUN_TO_BALL && !player.currentPlayer)
			{
				Vector2 directionToBall = (ball.pos - player.pos).norm();
				player.movePlayer(directionToBall);
				if (abs((ball.pos - player.pos).x) < 2 * V && abs((ball.pos - player.pos).y) < 2 * V)
					player.stopPlayer();
			}
			else if (!player.currentPlayer)
			{
				Vector2 directionToZoneCenter = ((player.zone_end + player.zone_begin) / 2 - player.pos).norm();
				player.movePlayer(directionToZoneCenter);
				if (abs(((player.zone_end + player.zone_begin) / 2 - player.pos).x) < V  && abs(((player.zone_end + player.zone_begin) / 2 - player.pos).y) < V)
					player.stopPlayer();
			}
		}
	}
}

void Player::stopPlayer()
{
	acceleration = Vector2(0, 0);
	velocity = Vector2(0, 0);
}

void Player::movePlayer(const Vector2 & direction)
{
	velocity = V * direction;
}

void Player::acceleratePlayer()
{
	acceleration = MAX_PLAYER_ACCELERATION * velocity.norm();
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
		currentFrame = 0;

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

void Team::createTeam(const char teamID, const Vector2& fieldSize)
{
	for (int i = 0; i < PLAYERS_AMOUNT; i++)
	{
		Player player;
		player.withBall = false;
		player.currentPlayer = false;
		player.stopPlayer();
		player.teamID = teamID;
		player.currentFrame = 0;
		player.texture.loadFromFile("player" + std::to_string(teamID) + "_stop.png");
		player.texture.setSmooth(true);
		player.pos = Vector2(i * 100 + 50, i * 100 + 50);

		players.push_back(player);
	}

	//defenders
	players[0].zone_begin = Vector2((teamID - 1) * fieldSize.x / 2, 0);
	players[0].zone_end = Vector2(fieldSize.x / 2 + (teamID - 1) * fieldSize.x / 2, fieldSize.y / 3);

	players[1].zone_begin = Vector2((teamID - 1) * fieldSize.x / 2, 2 * fieldSize.y / 3);
	players[1].zone_end = Vector2(fieldSize.x / 2 + (teamID - 1) * fieldSize.x / 2, fieldSize.y);

	players[2].zone_begin = Vector2((teamID - 1) * 3 * fieldSize.x / 5, fieldSize.y / 4);
	players[2].zone_end = Vector2(2 * fieldSize.x / 5 + (teamID - 1) * 3 * fieldSize.x / 5, 3 * fieldSize.y / 4);
	//mildfielder
	players[3].zone_begin = Vector2(fieldSize.x / 5, 0);
	players[3].zone_end = Vector2(4 * fieldSize.x / 5, fieldSize.y);
	//forward
	players[4].zone_begin = Vector2(fieldSize.x / 2 - (teamID - 1) * fieldSize.x / 2, 0);
	players[4].zone_end = Vector2(fieldSize.x - (teamID - 1) * fieldSize.x / 2, fieldSize.y / 2);

	players[5].zone_begin = Vector2(fieldSize.x / 2 - (teamID - 1) * fieldSize.x / 2, fieldSize.y / 2);
	players[5].zone_end = Vector2(fieldSize.x - (teamID - 1) * fieldSize.x / 2, fieldSize.y);

}

void Map::update(float dt)
{
	opponentTeam.setPositions(ball);
	myTeam.setPositions(ball);
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

	for (auto& hero : opponentTeam.players)
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
