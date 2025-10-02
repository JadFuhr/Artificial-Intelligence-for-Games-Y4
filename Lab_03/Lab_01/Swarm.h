#pragma once
#ifndef SWARM_HPP
#define SWARM_HPP

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Boid.h"


class Swarm
{
public:

	Swarm(unsigned int count);

	void update(float dt);
	void renderSwarm(sf::RenderWindow& window);
	void respawn(unsigned int count, sf::Vector2u windowSize);


private:

	std::vector<Boid> boids;

	float a;	// attraction strength 
	float b;	// repulsion strength
	float n;	// attraction attenuation
	float m;	// repulsion attenuation

	sf::Vector2f computeLJF(const Boid& me, const Boid& you);
};
#endif