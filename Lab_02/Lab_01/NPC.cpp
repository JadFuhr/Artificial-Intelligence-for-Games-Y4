#include <iostream>
#include <cmath>
#include "NPC.h"

NPC::NPC(float size)
{
	npc.setSize(sf::Vector2f(size, size));
	npc.setFillColor(sf::Color::White);
	npc.setPosition(sf::Vector2f(700.0f, 600.0f));
	npc.setOrigin(sf::Vector2f(size / 2.0f, size / 2.0f));

	setupNPCSprite();
	setUpVisionCone();
}

void NPC::npcUpdate(sf::Time dt, const Player& player)
{
	sf::Vector2f pos = npc.getPosition();


	trackPlayer(dt, player);
	updateVisionCone();

	npc_sprite.setPosition(pos);
	npc_sprite.rotate(sf::degrees(5));

	if (playerInVision(player))
	{
		visionCone.setFillColor(sf::Color(255, 0, 0, 100));
	}
	else
	{
		visionCone.setFillColor(sf::Color(0, 255, 0, 100));
	}
}


void NPC::npcRender(sf::RenderWindow& window)
{

	window.draw(npc);
	window.draw(npc_sprite);
	window.draw(visionCone);
}

void NPC::trackPlayer(sf::Time dt, const Player& player)
{

	sf::Vector2f playerPos = player.getPosition();
	sf::Vector2f npcPos = npc.getPosition();

	sf::Vector2f moveDir = playerPos - npcPos;
	float length = std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);


	if (length != 0)
	{
		moveDir /= length;
		npc.move(moveDir * speed * dt.asSeconds());
		facingDir = moveDir;
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


}

void NPC::setUpVisionCone()
{

	visionCone.setPointCount(3);
	visionCone.setPoint(0, sf::Vector2f(0.f, 0.f));                  
	visionCone.setPoint(1, sf::Vector2f(coneLength, -coneLength / 2)); 
	visionCone.setPoint(2, sf::Vector2f(coneLength, coneLength / 2));  
	visionCone.setFillColor(sf::Color::Green);
	visionCone.setOrigin(sf::Vector2f(0.f, 0.f));

}

void NPC::updateVisionCone()
{

	visionCone.setPosition(npc.getPosition());

	float angle = std::atan2(facingDir.y, facingDir.x) * 180.f / 3.14159f;
	visionCone.setRotation(sf::degrees(angle));

}

bool NPC::playerInVision(const Player& player)
{
	sf::Vector2f npcPos = npc.getPosition();
	sf::Vector2f playerPos = player.getPosition();

	sf::Vector2f toPlayer = playerPos - npcPos;
	float dist = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);
	if (dist > coneLength) return false; // too far

	// Normalize
	float facingLen = std::sqrt(facingDir.x * facingDir.x + facingDir.y * facingDir.y);
	if (facingLen == 0) return false; // no facing direction

	sf::Vector2f normFacing = facingDir / facingLen;
	sf::Vector2f normToPlayer = toPlayer / dist;

	// Dot product (clamped to avoid errors)
	float dot = normFacing.x * normToPlayer.x + normFacing.y * normToPlayer.y;
	dot = std::max(-1.f, std::min(1.f, dot));

	// Convert to angle
	float angle = std::acos(dot) * 180.f / 3.14159f;

	return angle < coneAngle;
}



