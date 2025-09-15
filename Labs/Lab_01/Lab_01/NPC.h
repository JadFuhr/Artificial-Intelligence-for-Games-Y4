#pragma once
#ifndef NPC_HPP
#define NPC_HPP
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


class NPC 
{
public:
	NPC(float size = 25.0f);

	void npcUpdate(sf::Time dt);
	void npcRender(sf::RenderWindow& window);

private:

	sf::RectangleShape npc;
	float speed{ 150.0f };
	sf::Vector2f direction{ 1.0f,1.0f };

};

#endif
