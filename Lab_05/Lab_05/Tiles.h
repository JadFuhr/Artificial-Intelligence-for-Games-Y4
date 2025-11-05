#pragma once
#ifndef TILES_HPP
#pragma once
#include <SFML/Graphics.hpp>

struct Tile
{
    sf::RectangleShape shape;
    bool traversable = true;
    int cost = 1;
    sf::Vector2f flow{ 0.0f,0.0f }; // unit vector (dir to best neighbour)

    // integration cost field
    // vector field

    Tile(float x, float y, float size);

};

#endif // !TILES_HPP
