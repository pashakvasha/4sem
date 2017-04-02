#include <SFML\Graphics.hpp>
#include "Vector2.h"
#include "logic.h"

extern int CURRENT_PLAYER;

void drawTeam(const std::vector<Player>& team, sf::RenderWindow& window, sf::Sprite & pointer, bool isOurTeam)
{
	for (int i = 0; i < team.size(); i++)
	{
		sf::Sprite man(team[i].texture);
		man.setPosition(team[i].pos.x, team[i].pos.y);
		man.setScale(team[i].size, team[i].size);
		man.setOrigin(team[i].texture.getSize().x / 2, team[i].texture.getSize().x / 2);

		if (i == CURRENT_PLAYER && isOurTeam)
		{
			pointer.setPosition(team[i].pos.x, team[i].pos.y - team[i].radius.y - 0.3 * team[i].size * pointer.getTexture()->getSize().y);
			pointer.setScale(0.3 * team[i].size, 0.3 * team[i].size);
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