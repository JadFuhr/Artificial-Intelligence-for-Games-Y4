#include <iostream>
#include "NPC.h"

NPC::NPC(float size)
{
	npc.setSize(sf::Vector2f(size, size));
	npc.setFillColor(sf::Color::White);
	npc.setPosition(sf::Vector2f(700.0f, 600.0f));
}

void NPC::npcUpdate(sf::Time dt)
{



}

void NPC::npcRender(sf::RenderWindow& window)
{

	window.draw(npc);

}


