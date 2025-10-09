#include "Formation.h"
#include <cmath>


static Vector2D rotateOffset(const Vector2D& offset, float angle)   // Helper function to rotate an offset vector by an angle (radians)
{
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);
    return {
        offset.x * cosA - offset.y * sinA,
        offset.x * sinA + offset.y * cosA
    };
}

Formation::Formation()
{
    anchor.setIsAnchor(true);
}

void Formation::initialize(Vector2D startPos)
{
    
    anchor = Agent(startPos, sf::Color::Red);   // Create anchor (red circle)
    anchor.setIsAnchor(true);
    anchorPosition = startPos;

    // Create 4 formation members (followers)
    members.clear();
    for (size_t i = 0; i < offsets.size(); i++)
    {
        sf::Color color = (i < 2) ? sf::Color::Green : sf::Color::Blue;
        members.emplace_back(startPos + offsets[i], color);
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

    // Normalize input direction
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

    // If moving, calculate facing direction (angle)
    if (movement.x != 0.f || movement.y != 0.f)
    {
        anchorAngle = std::atan2(movement.y, movement.x);
    }

    // Move anchor
    anchorPosition += movement * speed * dt;
    anchor.setPosition(anchorPosition);

    // Update followers to maintain rotated formation offsets
    for (size_t i = 0; i < members.size(); i++)
    {
        // Rotate the offset based on the anchor's facing direction
        Vector2D rotatedOffset = rotateOffset(offsets[i], anchorAngle);

        // target position for member
        Vector2D desiredPos = anchorPosition + rotatedOffset;

        // move member toward the desired position
        members[i].setTarget(desiredPos);
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
