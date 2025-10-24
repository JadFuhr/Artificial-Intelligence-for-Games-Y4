#pragma once
#ifndef TILES_HPP
#pragma once
#include <SFML/Graphics.hpp>

struct Tile
{
    sf::RectangleShape shape;
    bool traversable = true;
    int cost = 1;

    // integration cost field
    // vector field

    Tile(float x, float y, float size);

};

#endif // !TILES_HPP
