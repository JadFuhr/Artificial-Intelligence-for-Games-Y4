#include <iostream>
#include "Player.h"



Player::Player(float size)
{
	player.setSize(sf::Vector2f(size, size));
	player.setFillColor(sf::Color::Transparent);
	//player.setOutlineColor(sf::Color::White);
	//player.setOutlineThickness(2);
	player.setOrigin(sf::Vector2f(size / 2.0f, size / 2.0f));
	player.setPosition(sf::Vector2f(200, 200));

	setupSprite();
}


void Player::handleInput(sf::Time dt)
{
	// forward acceleration

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		speed += acceleration * dt.asSeconds();

		if (speed > maxSpeed)
		{
			speed = maxSpeed;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		speed -= acceleration * dt.asSeconds();

		if (speed < -maxSpeed / 2)
		{
			speed = -maxSpeed / 2;
		}
	}
	else
	{
		if (speed > 0)
		{
			speed -= deceleration * dt.asSeconds();
		}
		else if (speed < 0)
		{
			speed += deceleration * dt.asSeconds();
		}
	}

	// Rotate Player

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		player.rotate(sf::degrees(-rotationSpeed * dt.asSeconds()));
		player_sprite.rotate(sf::degrees(-rotationSpeed * dt.asSeconds()));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		player.rotate(sf::degrees(rotationSpeed * dt.asSeconds()));
		player_sprite.rotate(sf::degrees(rotationSpeed * dt.asSeconds()));
	}

}

void Player::playerUpdate(sf::Time dt)
{

	// Direction based on rotation
	float rotationRad = player.getRotation().asRadians();
	sf::Vector2f direction(std::cos(rotationRad), std::sin(rotationRad));

	// move player 
	player.move(direction * speed * dt.asSeconds());

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
		pos.y = -size.y;
	}

	player.setPosition(pos);
	player_sprite.setPosition(pos);
}


void Player::playerRender(sf::RenderWindow& window)
{

	window.draw(player);
	window.draw(player_sprite);
}

void Player::setupSprite()
{
	if (!player_texture.loadFromFile("ASSETS\\IMAGES\\Ship.png"))
	{
		std::cout << "problem loading player sprite" << std::endl;
	}
	
	player_sprite = sf::Sprite(player_texture);
	player_sprite.setOrigin(sf::Vector2f(player_texture.getSize().x / 2.0f, player_texture.getSize().y / 2.0f));
	player_sprite.rotate(sf::degrees(90));
}
