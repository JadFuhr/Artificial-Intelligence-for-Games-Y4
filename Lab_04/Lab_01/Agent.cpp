#include "Agent.h"

Agent::Agent()
{
    position = { 0, 0 };
    velocity = { 0, 0 };
    orientation.angle = 0;
    target = position;

    shape.setRadius(10.f);
    shape.setOrigin(sf::Vector2f(10.f, 10.f));
    shape.setFillColor(sf::Color::White);
    shape.setPosition(sf::Vector2f(position.x, position.y));
}

Agent::Agent(Vector2D pos, sf::Color colour)
{
    position = pos;
    velocity = { 0, 0 };
    orientation.angle = 0;
    target = pos;

    shape.setRadius(10.f);
    shape.setOrigin(sf::Vector2f(10.f, 10.f));
    shape.setFillColor(colour);
    shape.setPosition(sf::Vector2f(position.x, position.y));
}

void Agent::setIsAnchor(bool anchor)
{
    isAnchor = anchor;
}

void Agent::setTarget(Vector2D targetPos)
{
    target = targetPos;
}

Vector2D Agent::getPosition() const
{
    return position;
}

void Agent::update(float dt)
{
    if (!isAnchor)
    {
        Vector2D toTarget = target - position;
        float distance = toTarget.getLength();

        if (distance > 1.0f)
        {
            toTarget.normalize();
            velocity = toTarget * maxSpeed;
        }
        else
        {
            velocity = { 0, 0 };
        }
    }

    position += velocity * dt;
    shape.setPosition(sf::Vector2f(position.x, position.y));
}

void Agent::render(sf::RenderWindow& window)
{
    window.draw(shape);
}