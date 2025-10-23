#include "Tiles.h"
#include <iostream>

Tile::Tile(float x, float y, float size)
{
    shape.setSize({ size - 1, size - 1 }); // -1 for grid lines
    shape.setPosition(sf::Vector2f(x, y));
    shape.setFillColor(sf::Color::White);
}
