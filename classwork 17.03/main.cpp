#include <SFML\Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include "Vector2.h"
#include "logic.h"

std::vector<sf::Color> colors = {
	sf::Color(100, 0, 0),
	sf::Color(0, 100, 0),
	sf::Color(0, 0, 100),
	sf::Color(100, 0, 100),
	sf::Color(0, 100, 100)
};

int main()
{
	Map map;
	map.size = sf::Vector2i(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
	sf::CircleShape circle(50);
	float prev_time;
	

	sf::RenderWindow window(sf::VideoMode(map.size.x, map.size.y), "My window");
	sf::Clock clock;

	while (window.isOpen())
	{
		window.clear(sf::Color(16, 20, 25));
		
		sf::Time time = clock.getElapsedTime();
		sf::Event event;
		
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed: {
				window.close();
			} break;

			case sf::Event::MouseButtonPressed:
			{
				if (event.key.code == sf::Mouse::Left)
				{
					Ball ball;
					ball.position = Vector2(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
					ball.velocity = Vector2(rand() % MAX_VELOCITY, rand() % MAX_VELOCITY).rotate(rand() % 360);
					ball.type = rand() % 5;
					ball.radius = ball.type * MIN_RADUIS;
					ball.mass = ball.radius;
					map.balls.push_back(ball);
				}

			} break;
			default: break;
			}
		}

		map.update(time.asSeconds() - prev_time);
		prev_time = time.asSeconds();
	
		for (const auto& b : map.balls)
		{
			circle.setPosition(b.position.x, b.position.y);
			circle.setFillColor(colors[b.type]);
			circle.setRadius(b.radius);
			window.draw(circle);
		}

		window.display();
	}
	return 0;
}
