#pragma once
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Constants.h"

class Player
{
public:
	Player(float size = 50.0f);

	void handleInput(sf::Time dt);
	void playerUpdate(sf::Time dt);
	void playerRender(sf::RenderWindow& window);

	void setupSprite();

	sf::Vector2f getPosition() const;

	void setupPlayerSprite();

private:

	sf::RectangleShape player;
	sf::Texture player_texture;
	sf::Sprite player_sprite{ player_texture };


	float speed{ 200.0f };
	float acceleration{ 150.0f };
	float deceleration{ 100.0f };
	float maxSpeed{ 600.0f };
	float rotationSpeed{ 150.0f };
};

#endif 