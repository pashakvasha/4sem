#include <SFML\Graphics.hpp>
#include "Vector2.h"
#include "logic.h"

void Map::drawField(sf::RenderWindow& window)
{
	scoreboard = sf::Text("You " + std::to_string(myTeam.goalAmount) + " : " + std::to_string(opponentTeam.goalAmount) + " Opponent\nTime: " + std::to_string((int)time / 2), font);
	scoreboard.setCharacterSize(15);
	scoreboard.setStyle(sf::Text::Bold);
	scoreboard.setStyle(sf::Text::Underlined);
	scoreboard.setColor(sf::Color::White);
	scoreboard.setPosition(camera.pos.x - WINDOW_SIZE.x / 2 + scoreboard.getCharacterSize(), camera.pos.y - WINDOW_SIZE.y / 2 + scoreboard.getCharacterSize());

	sf::Sprite field(texture);
	field.setScale(2.0f, 1.76f);
	window.setView(camera.view);
	window.clear(sf::Color::Black);
	window.draw(field);
	myTeam.drawTeam(window);
	opponentTeam.drawTeam(window);
	ball.drawBall(window);
	window.draw(scoreboard);

}

void Team::drawTeam(sf::RenderWindow& window)
{
	sf::Sprite pointer(pointerTexture);
	for (int i = 0; i < players.size(); i++)
	{
		sf::Sprite man(players[i].texture);
		man.setPosition(players[i].pos.x, players[i].pos.y);
		man.setScale(players[i].size, players[i].size);
		man.setOrigin(players[i].texture.getSize().x / 2, players[i].texture.getSize().x / 2);

		if (i == currentPlayer)
		{
			pointer.setPosition(players[i].pos.x, players[i].pos.y - players[i].radius.y - 0.3 * players[i].size * pointer.getTexture()->getSize().y);
			pointer.setScale(0.3 * players[i].size, 0.3 * players[i].size);
			window.draw(pointer);
		}

		window.draw(man);
	}
}

void Ball::drawBall(sf::RenderWindow& window)
{
	sf::Sprite ballSprite(texture);
	ballSprite.setScale(0.1 * size, 0.1 * size);
	ballSprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
	ballSprite.setPosition(pos.x, pos.y);
	ballSprite.setRotation(angle);
	window.draw(ballSprite);
}
