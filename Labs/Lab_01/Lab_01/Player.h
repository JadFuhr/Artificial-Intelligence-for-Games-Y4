#pragma once
#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


class Player
{
public:
	Player(float size = 50.0f);

	void handleInput();
	void playerUpdate(sf::Time dt);
	void playerRender(sf::RenderWindow& window);

private:

	sf::RectangleShape player;
	float speed{ 200.0f };

};

#endif 