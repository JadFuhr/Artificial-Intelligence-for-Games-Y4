#include "Boid.h"
#include <cmath>

Boid::Boid(float radius)
{

	shape.setRadius(radius);
	shape.setOrigin(sf::Vector2f(radius, radius));
	shape.setFillColor(sf::Color::White);

	position = sf::Vector2f(rand() % 800, rand() % 600);	// rand pos
	velocity = sf::Vector2f((rand() % 100 - 50) / 10.f, (rand() % 100 - 50) / 10.f);		// rand initial velocity in direction
	acceleration = sf::Vector2f(0.f, 0.f);		// initial accel = 0, accel update each frame later 

}

void Boid::updateBoid(float dt)
{



}

void Boid::renderBoid(sf::RenderWindow& window)
{

	window.draw(shape);

}

void Boid::applyForce(const sf::Vector2f& force)
{



}

void Boid::setBoidPositon(const sf::Vector2f& pos)
{

	position = pos;
	shape.setPosition(position);

}

sf::Vector2f Boid::getBoidPosition() const
{
	return position;
}

void Boid::setBoidVelocity(const sf::Vector2f& vel)
{

	velocity = vel;

}

sf::Vector2f Boid::getBoidVelocity() const
{
	return velocity;
}

