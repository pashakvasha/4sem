#include <SFML\Graphics.hpp>
#include "Vector2.h"
#include "logic.h"

extern int PREVIOUS_BALL_PLAYER;

//IT IS ESSENTIAL to rewrite it, using enum, but I don't know syntax
char getDirection(const Vector2& v)
{
	Vector2 right(1, 0);
	if (v.len() == 0)
		return 0;
	double cos = (right * v) / (right.len() * v.len());
	if (cos > sqrt(2) / 2 && cos <= 1)
		return 1; // right;
	if (cos >= -1 && cos < -sqrt(2) / 2)
		return 2; // left
	if (cos > -sqrt(2) / 2 && cos <= sqrt(2) / 2 && v.x > 0)
		return 3; // up
	return 4; //down	
}

bool Line::isPointOnLine(const Vector2& point)
{
	Vector2 directionToPoint = point - begin;
	Vector2 line = end - begin;
	if ( abs(directionToPoint ^ line) < 500)
		return true;
	return false;
}

void Ball::createBall()
{
	texture.loadFromFile("ball.png");
	texture.setSmooth(true);
	pos = Vector2(640, 275);
	angle = 0;
	velocity = Vector2(0, 0);
	acceleration = Vector2(0, 0);
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

void Ball::checkFieldBoundary(const Vector2& boundary)
{
	if (pos.x + radius.x >= boundary.x && velocity.x > 0)
		velocity = Vector2(0, 0);
	if (pos.x - radius.x <= 0 && velocity.x < 0)
		velocity = Vector2(0, 0);
	if (pos.y - radius.y <= 0 && velocity.y < 0)
		velocity = Vector2(0, 0);
	if (pos.y + radius.y >= boundary.y && velocity.y > 0)
		velocity = Vector2(0, 0);
}

void Ball::moveBall(const Vector2& direction)
{
	velocity = MAX_BALL_VELOCITY * direction;
	acceleration = -MAX_BALL_ACCELERATION * velocity.norm();
}

bool Ball::in_zone(struct Player hero)
{
	if ((pos.x < hero.zone_end.x && pos.x > hero.zone_begin.x) && (pos.y < hero.zone_end.y && pos.y > hero.zone_begin.y))
		return true;
	return false;
}

bool Player::in_zone()
{
	if ((pos.x < zone_end.x && pos.x > zone_begin.x) && (pos.y < zone_end.y && pos.y > zone_begin.y))
		return true;
	return false;
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

	currentFrame += velocity.len() / 10 * dt;
	if (currentFrame > 4)
		currentFrame = 0;
}

void Player::setStartPosition()
{
	//IT IS ESSENTIAL to write normal start position
	pos = (zone_end + zone_begin) / 2;
}

void Player::setTexture(const PlayerTextures& textures)
{
	texture = textures.stop;
	char direction = getDirection(velocity);
	if (direction == 1)
		texture = textures.right[(int)currentFrame];
	if (direction == 2)
		texture = textures.left[(int)currentFrame];
	if (direction == 3)
		texture = textures.up[(int)currentFrame];
	if (direction == 4)
		texture = textures.down[(int)currentFrame];
}

void Team::createTeam(const char& ID, const Vector2& fieldSize)
{
	teamID = ID;
	downloadTextures();

	for (int i = 0; i < PLAYERS_AMOUNT; i++)
	{
		Player player;
		player.withBall = false;
		player.currentPlayer = false;
		player.stopPlayer();
		player.currentFrame = 0;
		player.texture.loadFromFile("player" + std::to_string(teamID) + "_stop.png");
		player.texture.setSmooth(true);
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
	//forwards
	players[4].zone_begin = Vector2(fieldSize.x / 2 - (teamID - 1) * fieldSize.x / 2, 0);
	players[4].zone_end = Vector2(fieldSize.x - (teamID - 1) * fieldSize.x / 2, fieldSize.y / 2);

	players[5].zone_begin = Vector2(fieldSize.x / 2 - (teamID - 1) * fieldSize.x / 2, fieldSize.y / 2);
	players[5].zone_end = Vector2(fieldSize.x - (teamID - 1) * fieldSize.x / 2, fieldSize.y);

	for (auto& player : players)
	{
		player.setStartPosition();
	}
}

void Team::update(Ball& ball, float dt)
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
		player.update(dt);
		player.setTexture(textures);
	}
}

void Team::downloadTextures()
{
	textures.stop.loadFromFile("player" + std::to_string(teamID) + "_stop.png");
	for (int i = 0; i < 4; i++) // 4 is the number  of frames
	{
		sf::Texture left;
		sf::Texture right;
		sf::Texture up;
		sf::Texture down;

		left.loadFromFile("player" + std::to_string(teamID) + "_left_" + std::to_string(i) + ".png");
		right.loadFromFile("player" + std::to_string(teamID) + "_right_" + std::to_string(i) + ".png");
		up.loadFromFile("player" + std::to_string(teamID) + "_up_" + std::to_string(i) + ".png");
		down.loadFromFile("player" + std::to_string(teamID) + "_down_" + std::to_string(i) + ".png");

		textures.left.push_back(left);
		textures.right.push_back(right);
		textures.up.push_back(up);
		textures.down.push_back(down);
	}
}

void Camera::setPosition(const Vector2& position)
{
	pos = position;
	if (pos.x <= 400)
		pos.x = 400;
	if (pos.x >= 880)
		pos.x = 880;
	pos.y = WINDOW_SIZE.y / 2;
	view.setCenter(pos.x, pos.y);
}

void Map::hit()
{
	Vector2 directionToGate = Vector2( (opponentGate.begin + opponentGate.end) / 2 - myTeam.players[myTeam.currentPlayer].pos).norm();
	ball.moveBall(directionToGate);

	myTeam.players[myTeam.currentPlayer].movePlayer(directionToGate);
	myTeam.players[myTeam.currentPlayer].withBall = false;
	withBall = false;
	PREVIOUS_BALL_PLAYER = myTeam.currentPlayer;
}

void Map::passToPlayer()
{
	Vector2 dir = myTeam.players[myTeam.currentPlayer].velocity;
	std::cout << dir << std::endl;
	double cos[PLAYERS_AMOUNT - 1];
	int in_zone[PLAYERS_AMOUNT - 1];
	int k = 0;
	int min;
	double k_min = -1;
	Vector2 ans;
	for (int i = 0; i < PLAYERS_AMOUNT; i++)
		if (i != myTeam.currentPlayer)
		{
			cos[i] = (dir * (myTeam.players[i].pos - ball.pos)) / (dir.len() * (myTeam.players[i].pos - ball.pos).len());
			if (cos[i] > k_min)
			{
				k_min = cos[i];
				min = i;
			}
			if ((cos[i] >= 0.7) && (cos[i] <= 1))
			{
				in_zone[k] = i;
				k++;
			}
		}
	float len = size.len();
	if (k != 0)
		for (int i = 0; i < k; i++)
		{
			if ((myTeam.players[i].pos - ball.pos).len() < len)
			{
				len = (myTeam.players[in_zone[i]].pos - ball.pos).len();
				min = in_zone[i];
			}
		}
	ans = myTeam.players[min].pos - ball.pos;
	ball.moveBall(ans.norm());
	myTeam.players[myTeam.currentPlayer].withBall = false;
	withBall = false;
	PREVIOUS_BALL_PLAYER = myTeam.currentPlayer;
}

void Map::changeCurrentPlayer()
{
	if (!withBall)
	{
		float distance = size.len();
		float current_distance;
		for (int i = 0; i < PLAYERS_AMOUNT; i++)
			if (i != myTeam.currentPlayer)
			{
				current_distance = (myTeam.players[i].pos - ball.pos).len();
				if (distance > current_distance)
				{
					distance = current_distance;
					myTeam.currentPlayer = i;
				}
			}
	}
	else
		myTeam.currentPlayer = myTeam.currentPlayer;
}

void Map::createField()
{
	font.loadFromFile("arial.ttf");

	texture.loadFromFile("field.png");
	texture.setSmooth(true);
	size = Vector2(2.0f * texture.getSize().x, 1.76f * texture.getSize().y);

	myTeam.pointerTexture.loadFromFile("pointer.png");
	myTeam.pointerTexture.setSmooth(true);


	myGate.begin = Vector2(70, 320);
	myGate.end = Vector2(100, 230);

	opponentGate.begin = Vector2(1210, 315);
	opponentGate.end = Vector2(1180, 230);

	camera.view.reset(sf::FloatRect(0, 0, WINDOW_SIZE.x, WINDOW_SIZE.y));
	myTeam.createTeam(1, size); // The first argument is teamID
	opponentTeam.createTeam(2, size);
	withBall = false;
	goal = false;
	myTeam.currentPlayer = 0;
	ball.createBall();
}

void Map::resetGame()
{
	ball.createBall();
	goal = false;
	withBall = false;
	myTeam.currentPlayer = 0;
	PREVIOUS_BALL_PLAYER = -1;
	for (size_t i = 0; i < myTeam.players.size(); i++)
	{
		myTeam.players[i].setStartPosition();
		opponentTeam.players[i].setStartPosition();
	}
}

void Map::update(float dt)
{
	//IT IS ESSENTIAL to add this cicle to setPositions;
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
	}

	opponentTeam.update(ball, dt);
	myTeam.update(ball, dt);

	for (int i = 0; (i < myTeam.players.size()) && (!withBall); i++)
	{
		if (i != PREVIOUS_BALL_PLAYER)
		{
			Vector2 d = ball.pos - myTeam.players[i].pos - Vector2(0, myTeam.players[i].radius.y);
			if (d.len() < ball.radius.x + myTeam.players[i].radius.x)
			{
				withBall = true;
				myTeam.players[i].withBall = true;
				myTeam.currentPlayer = i;
				PREVIOUS_BALL_PLAYER = -1;
			}
			else
			{
				withBall = false;
				myTeam.players[i].withBall = false;
			}
		}
	}
	if ( withBall )
	{
		ball.pos = myTeam.players[myTeam.currentPlayer].pos + Vector2(5, myTeam.players[myTeam.currentPlayer].radius.y);
		ball.velocity = myTeam.players[myTeam.currentPlayer].velocity;
	}

	if ( myGate.isPointOnLine(ball.pos) && (ball.pos.x >= 70) && (ball.pos.x <= 100) & (ball.pos.y >= 230) && (ball.pos.y <= 320))
	{
		goal = true;
		opponentTeam.goalAmount++;
	}

	if (opponentGate.isPointOnLine(ball.pos) && (ball.pos.x >= 1180) && (ball.pos.x <= 1210) & (ball.pos.y >= 230) && (ball.pos.y <= 315))
	{
		goal = true;
		myTeam.goalAmount++;
	}

	ball.checkFieldBoundary(size);
	ball.update(dt);

	camera.setPosition(ball.pos);

}
