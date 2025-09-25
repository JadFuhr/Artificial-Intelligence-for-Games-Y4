#include <iostream>
#include "NPC.h"

NPC::NPC(float size)
{
	npc.setSize(sf::Vector2f(size, size));
	npc.setFillColor(sf::Color::White);
	npc.setPosition(sf::Vector2f(700.0f, 600.0f));
	npc.setOrigin(sf::Vector2f(size / 2.0f, size / 2.0f));

	setupNPCSprite();
}

void NPC::npcUpdate(sf::Time dt, const Player& player)
{
	sf::Vector2f pos = npc.getPosition();


	trackPlayer(dt, player);

	npc_sprite.setPosition(pos);
	npc_sprite.rotate(sf::degrees(5));

	// Update vision cone
	updateVisionCone();

	// Check player detection
	if (playerInVision(player))
		visionCone.setFillColor(sf::Color(255, 0, 0, 100)); // red
	else
		visionCone.setFillColor(sf::Color(0, 255, 0, 100)); // green
}


void NPC::npcRender(sf::RenderWindow& window)
{
	window.draw(visionCone);
	window.draw(npc);
	window.draw(npc_sprite);

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

void NPC::setupNPCSprite()
{

	if (!npc_texture.loadFromFile("ASSETS\\IMAGES\\Alien.png"))
	{
		std::cout << "problem loading NPC sprite" << std::endl;
	}

	npc_sprite = sf::Sprite(npc_texture);
	npc_sprite.setOrigin(sf::Vector2f(npc_texture.getSize().x / 2, npc_texture.getSize().y / 2));
	npc_sprite.setPosition(sf::Vector2f(npc.getPosition().x, npc.getPosition().y));

	// Setup vision cone (triangle shape)
	visionCone.setPointCount(3);
	visionCone.setPoint(0, sf::Vector2f(0.f, 0.f));                  // NPC origin
	visionCone.setPoint(1, sf::Vector2f(coneLength, -coneLength / 2)); // right edge
	visionCone.setPoint(2, sf::Vector2f(coneLength, coneLength / 2));  // left edge
	visionCone.setFillColor(sf::Color(0, 255, 0, 100)); // semi-transparent green
	visionCone.setOrigin(sf::Vector2f(0.f, 0.f));
}



