#pragma once
#include "Agent.h"
#include "Vector2D.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

class Formation
{
public:
    Formation();

    void initialize(Vector2D startPos);
    void update(float dt);
    void render(sf::RenderWindow& window);
    void toggleVisible();

private:
    void handleInput();

    std::vector<Agent> members;
    Agent anchor;
    bool visible = true;

    std::vector<Vector2D> offsets = { {-30,30}, {30,30}, {-30,-30}, {30,-30} };
    Vector2D movement{ 0.f, 0.f };
    float speed = 150.f;

    Vector2D anchorPosition;
    float anchorAngle = 0.f; // rotation angle (radians)
};
