#pragma once
#include <SFML\Graphics.hpp>
#include "Vector2.h"

void drawTeam(const std::vector<Player>& team, sf::RenderWindow& window, sf::Sprite & pointer, bool isOurTeam);
void drawBall(const Ball ball, sf::RenderWindow& window);