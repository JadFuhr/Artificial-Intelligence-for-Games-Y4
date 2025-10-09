#include "Formation.h"
#include <cmath>

Formation::Formation()
{
    anchor.setIsAnchor(true);
}

void Formation::initialize(Vector2D startPos)
{
    anchor = Agent(startPos, sf::Color::Red);
    anchor.setIsAnchor(true);

    members.clear();
    for (size_t i = 0; i < offsets.size(); i++)
    {
        sf::Color color = (i < 2) ? sf::Color::Green : sf::Color::Blue;
        members.emplace_back(startPos + offsets[i], color);
    }

    // Set initial target to starting position
    anchorTarget = startPos;
    anchor.setTarget(anchorTarget);
    for (size_t i = 0; i < members.size(); i++)
    {
        members[i].setTarget(startPos + offsets[i]);
    }
}

void Formation::toggleVisible()
{
    visible = !visible;
}

void Formation::handleInput()
{
    movement = { 0.f, 0.f };
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) movement.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) movement.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) movement.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) movement.x += 1.f;

    // Normalize 
    float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
    if (length > 0.f)
    {
        movement.x /= length;
        movement.y /= length;
    }
}

void Formation::update(float dt)
{
    handleInput();

    // Move the anchor
    anchorTarget += movement * speed * dt;
    anchor.setTarget(anchorTarget);
    anchor.update(dt);

    // Update members relative to anchor
    Vector2D anchorPos = anchor.getPosition();
    for (size_t i = 0; i < members.size(); i++)
    {
        members[i].setTarget(anchorPos + offsets[i]);
        members[i].update(dt);
    }
}

void Formation::render(sf::RenderWindow& window)
{
    if (!visible) return;

    anchor.render(window);
    for (auto& m : members)
        m.render(window);
}