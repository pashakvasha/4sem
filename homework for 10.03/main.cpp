#include <SFML\Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <algorithm>
#include "logic.h"

int currentBul = 0;
bool MOUSE_RIGHT_PRESSED;

sf::Vector2f getRotated(float degree, sf::Vector2f vector);

int main()
{
	sf::Font font;
	font.loadFromFile("arial.ttf");

	float angle;
	Map map;
	map.size = sf::Vector2f(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
	map.hero.pos = sf::Vector2f(100, 100);
	map.hero.velocity = sf::Vector2f(0, 0);
	
	sf::RenderWindow window(sf::VideoMode(map.size.x, map.size.y), "My window");

	sf::Texture textureHero;
	textureHero.loadFromFile("blueship.png");
	sf::Sprite hero(textureHero);
	hero.setScale(0.45f, 0.45f);

	sf::Texture textureBullet;
	textureBullet.loadFromFile("bullet.png");
	sf::Sprite bul(textureBullet);
	bul.setScale(0.2f, 0.08f);

	sf::Texture textureLaser;
	textureLaser.loadFromFile("laser.png");
	sf::Sprite laser(textureLaser);
	laser.setScale(0.8f, 0.3f);

	sf::Vector2f b = (sf::Vector2f)bul.getTexture()->getSize();
	bul.setOrigin(b.x / 2, b.y / 2);

	map.hero.radius = (sf::Vector2f)hero.getTexture()->getSize();
	hero.setOrigin(map.hero.radius.x / 2, map.hero.radius.y / 2);

	while (window.isOpen())
	{

		sf::Text text("the rest of the bullet: " + std::to_string(MAX_BULLET_COUNT - currentBul), font);
		text.setCharacterSize(20);
		text.setStyle(sf::Text::Bold);
		text.setStyle(sf::Text::Underlined);
		text.setColor(sf::Color::Red);
		window.clear(sf::Color(16, 20, 25));

		sf::Event event;
		map.hero.velocity = sf::Vector2f(0, 0);
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed: {
					window.close();
				} break;

				case sf::Event::MouseButtonReleased:
				{
					if (event.key.code == sf::Mouse::Right)
					{
						MOUSE_RIGHT_PRESSED = false;
					}
				} break;

				/*
					event'ы работают медленнее, чем просто проверки isKeyPressed и т.п.
					т.е. если зажать кнопку и держать, то теоретически может лагать из-за этого
				*/
					
				case sf::Event::MouseButtonPressed:
				{
					if ( (event.key.code == sf::Mouse::Left) && (currentBul < MAX_BULLET_COUNT) )
					{
						Bullet bull;
						bull.pos = map.hero.pos;
						bull.velocity = getRotated(angle, sf::Vector2f(0, -MAX_BULLET_VELOCITY));
						bull.angle = angle;
						map.bullets.push_back(bull);
						currentBul++;
					}
					if (event.key.code == sf::Mouse::Right)
					{
						MOUSE_RIGHT_PRESSED = true;
					}
				} break;

				case sf::Event::KeyPressed:
				{
					if (event.key.code == sf::Keyboard::Left)
					{
						map.hero.velocity = sf::Vector2f(-MAX_HERO_VELOCITY, 0);
					}
					if (event.key.code == sf::Keyboard::Right)
					{
						map.hero.velocity = sf::Vector2f(MAX_HERO_VELOCITY, 0);
					}
					if (event.key.code == sf::Keyboard::Down)
					{
						map.hero.velocity = sf::Vector2f(0, MAX_HERO_VELOCITY);
					}
					if (event.key.code == sf::Keyboard::Up)
					{
						map.hero.velocity = sf::Vector2f(0, -MAX_HERO_VELOCITY);
					}
				} break;
			default:
				break;
			}
		}
		
		/*
		fixit: вам нужно изменить время dt, которое прошло за кадр.
		оно может отличаться между кадрами ... например, если вдруг что-то начнёт накатывать обновления неожиданное,
		то ваш герой будет двигаться медленнее, и пули тоже ... т.к. процессор будет активно занят чем-то ещё 
		*/
		map.update(1);

		sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
		sf::Vector2f center = hero.getPosition();
		sf::Vector2f d = sf::Vector2f(mousePosition.x, mousePosition.y) - center;
		angle = 90 + atan2f(d.y, d.x) * 180 / Pi;

		map.laser.pos = map.hero.pos;
		map.laser.angle = angle - 90;

		laser.setPosition(map.laser.pos);
		laser.setRotation(map.laser.angle);

		hero.setRotation(angle);
		hero.setPosition(map.hero.pos);

		// fixit: i < map.bullets.size() ... а ещё лучше for (const auto& b : map.bullets)
		for (int i = 0; i < currentBul; i++)
		{
			bul.setPosition(map.bullets[i].pos);
			bul.setRotation(map.bullets[i].angle);
			window.draw(bul);
		}
		window.draw(text);
		window.draw(hero);
		
		// лучше вместо этого флажка вызвать ф-ю ... вроде isMouseButtonPressed называется
		if (MOUSE_RIGHT_PRESSED)
		{
			window.draw(laser);
		}
		window.display();
	}
	return 0;
}
