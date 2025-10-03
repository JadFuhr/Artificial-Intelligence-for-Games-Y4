#include "Swarm.h"

Swarm::Swarm(unsigned int count): a(50.f), b(200.f), n(2), m(4) // tune these for behavior
{
    boids.reserve(count);

    for (unsigned int i = 0; i < count; ++i)
    {
        sf::Vector2f pos(static_cast<float>(std::rand() % 800), static_cast<float>(std::rand() % 600));
        boids.emplace_back(pos);
    }

}

void Swarm::update(float dt)
{

    for (size_t i = 0; i < boids.size(); ++i)
    {
        sf::Vector2f totalForce(0.f, 0.f);

        for (size_t j = 0; j < boids.size(); ++j)
        {
            if (i == j) continue;
            totalForce += computeLJF(boids[i], boids[j]);
        }

        boids[i].applyForce(totalForce);
    }

    for (auto& b : boids)
    {
        b.updateBoid(dt);
    }

}

void Swarm::renderSwarm(sf::RenderWindow& window)
{

    for (auto& b : boids)
    {
        b.renderBoid(window);
    }

}

void Swarm::respawn(unsigned int count, sf::Vector2u windowSize)
{

    boids.clear();
    boids.reserve(count);

    for (unsigned int i = 0; i < count; ++i)
    {
        sf::Vector2f pos(static_cast<float>(std::rand() % windowSize.x), static_cast<float>(std::rand() % windowSize.y));
        boids.emplace_back(pos);
    }

}

sf::Vector2f Swarm::computeLJF(const Boid& me, const Boid& you)
{
    sf::Vector2f r = me.getBoidPosition() - you.getBoidPosition();
    float d = std::sqrt(r.x * r.x + r.y * r.y);

    if (d < 1e-5f)
    {
        return { 0.f, 0.f }; // avoid divide by zero
    }


    // Potential energy U = -A/r^N + B/r^M
    float u = -a / std::pow(d, n) + b / std::pow(d, m);

    // Normalize R
    r.x /= d;
    r.y /= d;

    return { r.x * u, r.y * u };
}
