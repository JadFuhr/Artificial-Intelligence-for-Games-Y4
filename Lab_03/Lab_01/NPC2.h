#pragma once
#ifndef NPC2_HPP
#define NPC2_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include <cmath>

class NPC2
{
public:
	NPC2(float size = 25.0f);

	void npc2Update(sf::Time dt, const Player& player);
	void Wander(sf::Time dt);
	void setupNPC2Sprite();
	void npc2Render(sf::RenderWindow& window);
	void setUpVisionCone2();
	void toggleActive2();
	bool isActive2()const { return active; }
	sf::Vector2f rotateVector(sf::Vector2f& vec, float degrees);
private:

	sf::RectangleShape npc2;

	sf::Texture npc2_texture;

	sf::Sprite npc2_sprite{ npc2_texture };

	float speed{ 150.0f };
	sf::Vector2f direction{ 1.0f,1.0f };

	sf::ConvexShape visionCone;
	float coneLength{ 200.0f };   // how far NPC can see
	float coneAngle{ 45.0f };     // half-angle of vision in degrees

	sf::Vector2f facingDir{ 1.0f, 0.0f }; // initial facing direction (pointing right)

	float wanderOrientation = 0.0f;       // Current wander orientation
	float wanderRate = 0.5f;              // Maximum change in orientation per second
	float wanderOffset = 50.0f;           // Distance of circle in front of NPC
	float wanderRadius = 30.0f;           // Radius of the wander circle

	void updateVisionCone2();
	bool playerInVision2(const Player& player);

	bool active = false;

};

#endif