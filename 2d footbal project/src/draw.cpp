#include <SFML\Graphics.hpp>
#include "Vector2.h"
#include "logic.h"

void drawTeam(const Team& team, sf::RenderWindow& window, sf::Sprite & pointer, bool isOurTeam)
{
	for (int i = 0; i < team.players.size(); i++)
	{
		sf::Sprite man(team.players[i].texture);
		man.setPosition(team.players[i].pos.x, team.players[i].pos.y);
		man.setScale(team.players[i].size, team.players[i].size);
		man.setOrigin(team.players[i].texture.getSize().x / 2, team.players[i].texture.getSize().x / 2);

		if (i == team.currentPlayer && isOurTeam)
		{
			pointer.setPosition(team.players[i].pos.x, team.players[i].pos.y - team.players[i].radius.y - 0.3 * team.players[i].size * pointer.getTexture()->getSize().y);
			pointer.setScale(0.3 * team.players[i].size, 0.3 * team.players[i].size);
			window.draw(pointer);
		}

		window.draw(man);
	}
}

void drawBall(const Ball ball, sf::RenderWindow& window)
{
	sf::Sprite ballSprite(ball.texture);
	ballSprite.setScale(0.1 * ball.size, 0.1 * ball.size);
	ballSprite.setOrigin(ball.texture.getSize().x / 2, ball.texture.getSize().y / 2);
	ballSprite.setPosition(ball.pos.x, ball.pos.y);
	ballSprite.setRotation(ball.angle);
	window.draw(ballSprite);
}
