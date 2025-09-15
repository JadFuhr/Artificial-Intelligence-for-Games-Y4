#include <iostream>
#include "Player.h"



Player::Player(float size)
{
	player.setSize(sf::Vector2f(size, size));
	player.setFillColor(sf::Color::Black);
	player.setOutlineColor(sf::Color::White);
	player.setOutlineThickness(2);
	player.setPosition(sf::Vector2f(200, 200));
}


void Player::handleInput()
{



}

void Player::playerUpdate(sf::Time dt)
{



}


void Player::playerRender(sf::RenderWindow& window)
{

	window.draw(player);


}
