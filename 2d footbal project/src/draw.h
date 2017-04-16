#pragma once
#include <SFML\Graphics.hpp>
#include "Vector2.h"
#include "logic.h"

void drawTeam(const Team& team, sf::RenderWindow& window, sf::Sprite & pointer, bool isOurTeam);
void drawBall(const Ball ball, sf::RenderWindow& window);