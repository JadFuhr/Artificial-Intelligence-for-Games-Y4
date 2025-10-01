#include "Boid.h"

Boid::Boid(float radius)
{



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

