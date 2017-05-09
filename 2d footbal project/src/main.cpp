#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "Vector2.h"
#include "logic.h"

int PREVIOUS_BALL_PLAYER = -1;

int main()
{

	sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "My window");
	sf::Clock clock;

	float last_time = 0;
	float dt;

	sf::Event event;

	Map map;
	map.createField();

	while (window.isOpen())
	{
		
		for (int i = 0; i < PLAYERS_AMOUNT; i++)
		{
			map.myTeam.players[i].currentPlayer = false;
			map.myTeam.players[i].stopPlayer();
			map.opponentTeam.players[i].stopPlayer();
		}
		map.myTeam.players[map.myTeam.currentPlayer].currentPlayer = true;

		sf::Time time = clock.getElapsedTime();
		sf::Event event;

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

				if (event.key.code == sf::Keyboard::A && map.withBall)
				{
					map.hit();
				}

			}
			default:
				break;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && map.myTeam.players[map.myTeam.currentPlayer].velocity.len() != 0)
		{
			map.myTeam.players[map.myTeam.currentPlayer].acceleratePlayer();
		}

		dt = time.asSeconds() - last_time;
		map.time += dt;
		map.update(dt);

		map.drawField(window);		

		if (map.goal)
		{
			sf::Text text_goal("Goooooal", map.font);
			sf::Music music;
			if (!music.openFromFile("goal.wav"))
				std::cout << "ERROR";
			text_goal.setCharacterSize(55);
			text_goal.setStyle(sf::Text::Bold);
			text_goal.setColor(sf::Color::Yellow);
			text_goal.setPosition(map.camera.pos.x - text_goal.getCharacterSize(), map.camera.pos.y - text_goal.getCharacterSize());
			window.draw(text_goal);
			window.display();
			music.play();
			_sleep(2500);
			map.time -= 2;
			map.resetGame();
		}

		window.display();
		last_time = time.asSeconds();
	}

	return 0;
}
