#include <SFML\Graphics.hpp>
#include "Vector2.h"
#include "logic.h"
#include "draw.h"

//IT IS ESSENTIAL to try delete global variables
int PREVIOUS_BALL_PLAYER = -1;

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

	sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "My window");
	sf::Clock clock;

	float last_time = 0;
	float dt;

	sf::Event event;

	Map map;
	map.size = Vector2(2.0f * field.getTexture()->getSize().x, 1.76f * field.getTexture()->getSize().y);
	map.createGame();

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
		map.myTeam.players[map.myTeam.currentPlayer].currentPlayer = true;

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
					map.changeCurrentPlayer();
				}

				if (event.key.code == sf::Keyboard::D && map.withBall)
				{
					map.passToPlayer();
				}

			}
			default:
				break;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			map.myTeam.players[map.myTeam.currentPlayer].movePlayer(Vector2(-1, 0));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			map.myTeam.players[map.myTeam.currentPlayer].movePlayer(Vector2(1, 0));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			map.myTeam.players[map.myTeam.currentPlayer].movePlayer(Vector2(1, -1));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			map.myTeam.players[map.myTeam.currentPlayer].movePlayer(Vector2(-1, 1));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && map.myTeam.players[map.myTeam.currentPlayer].velocity.len() != 0)
		{
			map.myTeam.players[map.myTeam.currentPlayer].acceleratePlayer();
		}

		dt = time.asSeconds() - last_time;
		map.update(dt);

		window.setView(map.camera.view);
		window.clear(sf::Color::Black);
		window.draw(field);
		drawTeam(map.myTeam, window, pointer, true);
		drawTeam(map.opponentTeam, window, pointer, false);
		drawBall(map.ball, window);
		window.draw(text);
		window.display();

		last_time = time.asSeconds();
	}

	return 0;
}
