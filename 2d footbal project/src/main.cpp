#include <SFML\Graphics.hpp>
#include "Vector2.h"
#include "logic.h"
#include "draw.h"

int PREVIOUS_BALL_PLAYER = -1;
int BALL_PLAYER = -1;
int CURRENT_PLAYER = 0;


int main()
{
	sf::Font font;
	font.loadFromFile("arial.ttf");

	Map map;
	map.size = Vector2(800, 600);
	createBall(map.ball);
	createTeams(map);

	sf::RenderWindow window(sf::VideoMode(map.size.x, map.size.y), "My window");
	sf::Clock clock;

	float last_time = 0;
	float dt;

	sf::Texture texturePointer;
	texturePointer.loadFromFile("pointer.png");
	texturePointer.setSmooth(true);
	sf::Sprite pointer(texturePointer);

	sf::Texture textureField;
	textureField.loadFromFile("field.png");
	textureField.setSmooth(true);
	sf::Sprite field(textureField);
	field.setScale(1.25f, 1.75f);

	sf::Event event;
	while (window.isOpen())
	{	

		sf::Text text("Q - change player\n D - pass to another player", font);
		text.setCharacterSize(15);
		text.setStyle(sf::Text::Bold);
		text.setStyle(sf::Text::Underlined);
		text.setColor(sf::Color::White);

		for (int i = 0; i < map.players.size(); i++)
		{
			map.players[i].velocity = Vector2(0, 0);

			map.opponentPlayers[i].velocity = Vector2(0, 0);
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
					map.ball.acceleration = -MAX_BALL_ACCELERATION * map.ball.velocity.norm();
					BALL_PLAYER = -1;
					PREVIOUS_BALL_PLAYER = CURRENT_PLAYER;
				}
			}
			default:
				break;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			map.players[CURRENT_PLAYER].velocity = Vector2(-V, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			map.players[CURRENT_PLAYER].velocity = Vector2(V, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			map.players[CURRENT_PLAYER].velocity = V * Vector2(1, -1);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			map.players[CURRENT_PLAYER].velocity = -V * Vector2(1, -1);
		}

		

		dt = time.asSeconds() - last_time;
		map.update(dt);

		window.draw(field);
		drawTeam(map.players, window, pointer, true);
		drawTeam(map.opponentPlayers, window, pointer, false);
		drawBall(map.ball, window);
		
		window.draw(text);
		window.display();

		last_time = time.asSeconds();
		//std::cout << map.ball.pos << "   ";
		//std::cout << map.players[0].velocity << "\n";
		//std::cout << map.ball.radius << "\n";
		//std::cout << CURRENT_PLAYER << " ";
		//std::cout << BALL_PLAYER << std::endl;
		
	}
	return 0;
}
