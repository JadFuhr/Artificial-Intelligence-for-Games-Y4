#pragma once
#ifndef NPC_HPP
#define NPC_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"

class NPC 
{
public:
	NPC(float size = 25.0f);

	void npcUpdate(sf::Time dt, const Player& player);
	void npcRender(sf::RenderWindow& window);
	void trackPlayer(sf::Time dt, const Player& player);
	void setupNPCSprite();
	void setUpVisionCone();
	void toggleActive();
	bool isActive()const { return active; }

private:

	sf::RectangleShape npc;
	sf::Texture npc_texture;
	sf::Sprite npc_sprite{ npc_texture };

	float speed{ 150.0f };
	sf::Vector2f direction{ 1.0f,1.0f };

	sf::ConvexShape visionCone;
	float coneLength{ 200.0f };   // how far NPC can see
	float coneAngle{ 45.0f };     // half-angle of vision in degrees

	sf::Vector2f facingDir{ 1.0f, 0.0f }; // initial facing direction (pointing right)

	void updateVisionCone();
	bool playerInVision(const Player& player);

	bool active = false;

};

#endif
