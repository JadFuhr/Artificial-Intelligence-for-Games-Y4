#include "Boid.h")


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

	velocity += acceleration * dt;

	float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y); // clamp velocity

	if (speed > maxSpeed)
	{
		velocity *= maxSpeed / speed;
	}

	position += velocity * dt; 
	acceleration = sf::Vector2f(0.0f, 0.0f);

	shape.setPosition(position);

}

void Boid::renderBoid(sf::RenderWindow& window)
{

	window.draw(shape);

}

void Boid::applyForce(const sf::Vector2f& force)
{

	sf::Vector2f f = force;

	float magnitude = std::sqrt(f.x * f.x + f.y * f.y);	// clamp force

	if (magnitude > maxForce)
	{
		f *= maxForce / magnitude;
	}

	acceleration += f;

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

