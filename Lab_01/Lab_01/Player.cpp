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
	sf::Vector2f movement{ 0.0f,0.0f };

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) movement.x -= 1.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) movement.x += 1.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) movement.y -= 1.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) movement.y += 1.0f;

	player.move(movement * speed * (1.0f / 60.0f)); // per frame 
}

void Player::playerUpdate(sf::Time dt)
{
	sf::Vector2f pos = player.getPosition();
	sf::Vector2f size = player.getSize();

	// horizontal wrapping 

	if (pos.x + size.x < 0)
	{
		pos.x = WINDOW_X;
	}
	else if (pos.x > WINDOW_X)
	{
		pos.x = -size.x;
	}

	// vertical wrapping 

	if (pos.y + size.y < 0)
	{
		pos.y = WINDOW_Y;
	}
	else if (pos.y > WINDOW_Y)
	{
		pos.y = -size.y;a
	}

	player.setPosition(pos);

}


void Player::playerRender(sf::RenderWindow& window)
{

	window.draw(player);


}
