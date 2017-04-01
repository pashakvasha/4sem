#include <SFML\Graphics.hpp>
#include "Vector2.h"

const float Pi = 3.14159f;
const int indent = 20;
const float V = 4000;
int CURRENT_PLAYER = 0;
const int PLAYERS_AMOUNT = 4;
int BALL_PLAYER = -1;
int PREVIOUS_BALL_PLAYER = -1;
const int MAX_BALL_VELOCITY = 500;
const int MAX_BALL_ACCELERARION = 300;
float CURRENT_FRAME = 0;

struct Ball 
{
	Vector2 pos;
	Vector2 velocity;
	Vector2 acceleration;
	Vector2 radius;
	float size;
	void update(float dt)
	{
		pos += velocity * dt;
		if (velocity * acceleration < 0)
			velocity += acceleration * dt;
		else
			velocity = Vector2(0, 0);
	}
};

struct Player
{
	Vector2 pos;
	Vector2 velocity;
	Vector2 radius;
	sf::Texture texture;
	float size;
	void update(float dt)
	{
		pos += velocity * dt;
	}
};

struct Map
{
	Ball ball;
	std::vector<Player> players;
	std::vector<Player> opponentPlayers;
	Vector2 size;
	void update(float dt)
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
		
		for (int i = 0; ( i < players.size() ) && ( BALL_PLAYER < 0 ); i++)
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

};

int main()
{
	sf::Font font;
	font.loadFromFile("arial.ttf");

	Map map;
	map.size = Vector2(800, 600);
	map.ball.pos = Vector2(400, 400);
	map.ball.velocity = Vector2(0, 0);
	map.ball.acceleration = Vector2(0, 0);
	sf::RenderWindow window(sf::VideoMode(map.size.x, map.size.y), "My window");
	sf::Clock clock;

	float last_time = 0;

	sf::Texture texturePointer;
	texturePointer.loadFromFile("pointer.png");
	texturePointer.setSmooth(true);
	sf::Sprite pointer(texturePointer);

	sf::Texture textureField;
	textureField.loadFromFile("field.png");
	textureField.setSmooth(true);
	sf::Sprite field(textureField);
	field.setScale(1.25f, 1.75f);

	sf::Texture textureBall;
	textureBall.loadFromFile("ball.png");
	textureBall.setSmooth(true);
	sf::Sprite ball(textureBall);

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


	sf::Event event;
	while (window.isOpen())
	{	

		sf::Text text("Q - change player\n D - pass to another player ", font);
		text.setCharacterSize(20);
		text.setStyle(sf::Text::Bold);
		text.setStyle(sf::Text::Underlined);
		text.setColor(sf::Color::White);

		for (auto& hero : map.players)
		{
			hero.velocity = Vector2(0, 0);
		}

		sf::Time time = clock.getElapsedTime();
		sf::Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
			{
				if (event.key.code == sf::Keyboard::Left)
				{
					map.players[CURRENT_PLAYER].texture.loadFromFile("player1_left_0.png");
					map.players[CURRENT_PLAYER].velocity = Vector2(-V, 0);
				}
				if (event.key.code == sf::Keyboard::Right)
				{
					map.players[CURRENT_PLAYER].texture.loadFromFile("player1_right_0.png");
					map.players[CURRENT_PLAYER].velocity = Vector2(V, 0);
				}
				if (event.key.code == sf::Keyboard::Up)
				{
					map.players[CURRENT_PLAYER].texture.loadFromFile("player1_up_0.png");
					map.players[CURRENT_PLAYER].velocity = V * Vector2(1, -1);
				}
				if (event.key.code == sf::Keyboard::Down)
				{
					map.players[CURRENT_PLAYER].texture.loadFromFile("player1_down_0.png");
					map.players[CURRENT_PLAYER].velocity = -V * Vector2(1, -1);
				}
				if (event.key.code == sf::Keyboard::Q)
				{
					if (BALL_PLAYER < 0)
					{
						CURRENT_PLAYER++;
						if (CURRENT_PLAYER >= PLAYERS_AMOUNT)
							CURRENT_PLAYER = 0;
					}
					else
						CURRENT_PLAYER = BALL_PLAYER;
				}
				if (event.key.code == sf::Keyboard::D && BALL_PLAYER != -1)
				{
					int k = rand() % PLAYERS_AMOUNT;
					while ( k == CURRENT_PLAYER)
					{
						k = rand() % PLAYERS_AMOUNT;
					}
					map.ball.velocity = MAX_BALL_VELOCITY * (map.players[k].pos - map.players[CURRENT_PLAYER].pos).norm();
					map.ball.acceleration = -MAX_BALL_ACCELERARION * map.ball.velocity.norm();
					BALL_PLAYER = -1;
					PREVIOUS_BALL_PLAYER = CURRENT_PLAYER;
				}
			}
			default:
				break;
			}
		}
		/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			texturePlayer.loadFromFile("player1_left_1.png");
			texturePlayer.setSmooth(true);
			sf::Sprite man(texturePlayer);
		}*/

		map.update(time.asSeconds() - last_time);
		

		window.draw(field);
		
		for (int i = 0; i < map.players.size(); i++)
		{
			sf::Sprite man(map.players[i].texture);
			man.setPosition(map.players[i].pos.x, map.players[i].pos.y);
			map.players[i].size = (20 * map.players[i].pos.y / (map.size.y + 1)) * 0.02f + 0.3f;
			man.setScale(map.players[i].size, map.players[i].size);
			map.players[i].radius = Vector2(map.players[i].size * man.getTexture()->getSize().x / 2, map.players[i].size * man.getTexture()->getSize().y / 2);
			man.setOrigin(map.players[i].size * man.getTexture()->getSize().x / 2, map.players[i].size * man.getTexture()->getSize().y / 2);
			if (i == CURRENT_PLAYER)
			{
				pointer.setPosition(map.players[i].pos.x, map.players[i].pos.y - map.players[i].radius.y);
				pointer.setScale(0.3 * map.players[i].size, 0.3 * map.players[i].size);
				window.draw(pointer);
			}
			window.draw(man);
		}

		for (int i = 0; i < map.opponentPlayers.size(); i++)
		{
			sf::Sprite Opponent(map.opponentPlayers[i].texture);
			Opponent.setPosition(map.opponentPlayers[i].pos.x, map.opponentPlayers[i].pos.y);
			map.opponentPlayers[i].size = (20 * map.opponentPlayers[i].pos.y / (map.size.y + 1)) * 0.02f + 0.3f;
			Opponent.setScale(map.opponentPlayers[i].size, map.opponentPlayers[i].size);
			map.players[i].radius = Vector2(map.opponentPlayers[i].size * Opponent.getTexture()->getSize().x / 2, map.opponentPlayers[i].size * Opponent.getTexture()->getSize().y / 2);
			Opponent.setOrigin(map.opponentPlayers[i].size * Opponent.getTexture()->getSize().x / 2, map.opponentPlayers[i].size * Opponent.getTexture()->getSize().y / 2);
			window.draw(Opponent);
		}

		map.ball.size = (20 * map.ball.pos.y / (map.size.y + 1)) * 0.02f + 0.3f;
		map.ball.radius = Vector2(0.1 * map.ball.size * ball.getTexture()->getSize().x / 2, 0.1 * map.ball.size * ball.getTexture()->getSize().y / 2);
		ball.setScale(0.1 * map.ball.size, 0.1 * map.ball.size);
		ball.setOrigin(map.ball.size * ball.getTexture()->getSize().x / 2, map.ball.size * ball.getTexture()->getSize().y / 2);
		ball.setPosition(map.ball.pos.x, map.ball.pos.y);
		window.draw(ball);
		
		window.draw(text);
		window.display();

		last_time = time.asSeconds();
		/*std::cout << map.ball.pos << "   ";
		std::cout << map.players[0].pos << "   ";
		std::cout << map.players[0].radius << "   ";
		std::cout << map.ball.radius << "   ";*/
		std::cout << CURRENT_PLAYER << " ";
		std::cout << BALL_PLAYER << std::endl;
		
	}
	return 0;
}