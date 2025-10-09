#pragma once
#include "stdafx.h"
#include "Global.h"

class Agent
{
public:
    Agent();
    Agent(Vector2D pos, sf::Color colour = sf::Color::White);

    void update(float dt);
    void render(sf::RenderWindow& window);
    void setTarget(Vector2D targetPos);
    void setIsAnchor(bool anchor);
    Vector2D getPosition() const;

private:
    Vector2D position;
    Vector2D velocity;
    Orientation orientation;
    Vector2D target;

    float maxSpeed = 150.f;
    bool isAnchor = false;

    sf::CircleShape shape;
};