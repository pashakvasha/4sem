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

	sf::Texture texturePointer;
	texturePointer.loadFromFile("pointer.png");
	texturePointer.setSmooth(true);
	sf::Sprite pointer(texturePointer);

	sf::Texture textureField;
	textureField.loadFromFile("field.png");
	textureField.setSmooth(true);
	sf::Sprite field(textureField);
	field.setScale(2.0f, 1.76f);

	Map map;
	map.size = Vector2(2.0f * field.getTexture()->getSize().x, 1.76f * field.getTexture()->getSize().y);

	map.myTeam.createTeam(1, map.size);
	map.opponentTeam.createTeam(2, map.size);

	createBall(map.ball);

	sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "My window");
	sf::Clock clock;

	float last_time = 0;
	float dt;

	sf::Event event;

	sf::View view;
	view.reset(sf::FloatRect(0, 0, WINDOW_SIZE.x, WINDOW_SIZE.y));

	while (window.isOpen())
	{	

		sf::Text text("Q - change player\n D - pass to another player\n E - acceleration", font);
		text.setCharacterSize(15);
		text.setStyle(sf::Text::Bold);
		text.setStyle(sf::Text::Underlined);
		text.setColor(sf::Color::White);

		
		for (int i = 0; i < PLAYERS_AMOUNT; i++)
		{
			map.myTeam.players[i].currentPlayer = false;
			map.myTeam.players[i].stopPlayer();
			map.opponentTeam.players[i].stopPlayer();
		}

		map.myTeam.players[CURRENT_PLAYER].currentPlayer = true;

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
					Vector2 directionToPass = (map.myTeam.players[k].pos - map.myTeam.players[CURRENT_PLAYER].pos).norm();
					map.ball.moveBall(directionToPass);

					map.myTeam.players[CURRENT_PLAYER].movePlayer(directionToPass);
					map.myTeam.players[CURRENT_PLAYER].withBall = false;
					BALL_PLAYER = -1;
					PREVIOUS_BALL_PLAYER = CURRENT_PLAYER;
					//CURRENT_PLAYER = k;
				}
			}
			default:
				break;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			map.myTeam.players[CURRENT_PLAYER].movePlayer(Vector2(-1, 0));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			map.myTeam.players[CURRENT_PLAYER].movePlayer(Vector2(1, 0));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			map.myTeam.players[CURRENT_PLAYER].movePlayer(Vector2(1, -1));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			map.myTeam.players[CURRENT_PLAYER].movePlayer(Vector2(-1, 1));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && map.myTeam.players[CURRENT_PLAYER].velocity.len() != 0)
		{
			map.myTeam.players[CURRENT_PLAYER].acceleratePlayer();
		}
		
		map.camera.pos = map.ball.pos;

		dt = time.asSeconds() - last_time;
		map.update(dt);

		view.setCenter(map.camera.pos.x, map.size.y / 2);
		window.setView(view);
		window.clear(sf::Color::Black);
		window.draw(field);
		drawTeam(map.myTeam.players, window, pointer, true);
		drawTeam(map.opponentTeam.players, window, pointer, false);
		drawBall(map.ball, window);
		
		
		window.draw(text);
		window.display();

		last_time = time.asSeconds();
		//std::cout << map.ball.pos << "\n";
		//std::cout << map.myTeam.players[3].velocity << "\n";
		//std::cout << map.ball.radius << "\n";
		//std::cout << map.players[CURRENT_PLAYER].velocity.x << "\n ";
		//std::cout << BALL_PLAYER << std::endl;
		
	}
	return 0;
}
