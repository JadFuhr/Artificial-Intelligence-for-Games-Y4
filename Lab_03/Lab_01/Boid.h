#pragma once
#ifndef BOID_HPP
#define BOID_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Constants.h"
#include <cmath>

class Boid
{
public:

	Boid(const sf::Vector2f& startPos, float radius = 3.0f);

	void updateBoid(float dt);
	void renderBoid(sf::RenderWindow& window);
	void applyForce(const sf::Vector2f& force);

	void setBoidPositon(const sf::Vector2f& pos);
	sf::Vector2f getBoidPosition() const;

	void setBoidVelocity(const sf::Vector2f& vel);
	sf::Vector2f getBoidVelocity() const;


private:

	sf::CircleShape shape;
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;

	float maxSpeed = 200.0f;
	float maxForce = 400.0f;

};

#endif