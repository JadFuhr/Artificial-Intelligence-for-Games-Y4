#include <iostream>
#include "NPC.h"

NPC::NPC(float size)
{
	npc.setSize(sf::Vector2f(size, size));
	npc.setFillColor(sf::Color::White);
	npc.setPosition(sf::Vector2f(700.0f, 600.0f));
}

void NPC::npcUpdate(sf::Time dt, const Player& player)
{

	trackPlayer(dt, player);

}

void NPC::npcRender(sf::RenderWindow& window)
{

	window.draw(npc);

}

void NPC::trackPlayer(sf::Time dt, const Player& player)
{

	sf::Vector2f playerPos = player.getPosition();
	sf::Vector2f npcPos = npc.getPosition();

	sf::Vector2f direction = playerPos - npcPos;

	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	if (length != 0)
	{
		direction /= length;
		npc.move(direction * speed * dt.asSeconds());
	}



}



