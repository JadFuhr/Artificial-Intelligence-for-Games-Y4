#pragma once
#ifndef BOID_HPP
#define BOID_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Boid
{
public:

	Boid(float radius = 3.0f);

	void updateBoid(float dt);
	void renderBoid(sf::RenderWindow& window);
	void applyForce(const sf::Vector2f& force);

	void setBoidPositon(const sf::Vector2f& pos);
	sf::Vector2f getBoidPosition() const;

	void setBoidVelocity(const sf::Vector2f& velocity);
	sf::Vector2f getBoidVelocity() const;

	void wrapAround(float width, float height);


private:

	sf::CircleShape shape;
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;

	float maxSpeed = 100.0f;
	float maxForce = 200.0f;

};

#endif