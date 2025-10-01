#include <iostream>
#include <cmath>
#include "NPC2.h"


NPC2::NPC2(float size)
{
	npc2.setSize(sf::Vector2f(size, size));
	npc2.setFillColor(sf::Color::White);
	npc2.setPosition(sf::Vector2f(900.0f, 200.0f));
	npc2.setOrigin(sf::Vector2f(size / 2.0f, size / 2.0f));

	setupNPC2Sprite();
	setUpVisionCone2();
	
}

void NPC2::npc2Render(sf::RenderWindow& window)
{
	window.draw(npc2);
	window.draw(npc2_sprite);
	window.draw(visionCone);
}

void NPC2::setUpVisionCone2()
{


	visionCone.setPointCount(3);
	visionCone.setPoint(0, sf::Vector2f(0.f, 0.f));
	visionCone.setPoint(1, sf::Vector2f(coneLength, -coneLength / 2));
	visionCone.setPoint(2, sf::Vector2f(coneLength, coneLength / 2));
	visionCone.setFillColor(sf::Color(0, 255, 0, 100));
	visionCone.setOrigin(sf::Vector2f(0.f, 0.f));
	visionCone.setPosition(npc2.getPosition());

}

void NPC2::toggleActive2()
{

	active = !active;

}

sf::Vector2f NPC2::rotateVector(sf::Vector2f& vec, float degrees)
{
	float rad = degrees * 3.14159265f / 180.0f;
	float cs = std::cos(rad);
	float sn = std::sin(rad);
	return sf::Vector2f(vec.x * cs - vec.y * sn, vec.x * sn + vec.y * cs);
}

void NPC2::updateVisionCone2()
{

	if (active)
	{
		visionCone.setPosition(npc2.getPosition());

		float angle = std::atan2(facingDir.y, facingDir.x) * 180.f / 3.14159f;
		visionCone.setRotation(sf::degrees(angle));
	}



}

bool NPC2::playerInVision2(const Player& player)
{
	if (active)
	{
		sf::Vector2f npcPos = npc2.getPosition();
		sf::Vector2f playerPos = player.getPosition();

		sf::Vector2f toPlayer = playerPos - npcPos;
		float dist = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);
		if (dist > coneLength) return false; // too far

		// Normalize
		float facingLen = std::sqrt(facingDir.x * facingDir.x + facingDir.y * facingDir.y);
		if (facingLen == 0) return false; // no facing direction

		sf::Vector2f normFacing = facingDir / facingLen;
		sf::Vector2f normToPlayer = toPlayer / dist;

		// Dot product (clamped to avoid errors)
		float dot = normFacing.x * normToPlayer.x + normFacing.y * normToPlayer.y;
		dot = std::max(-1.f, std::min(1.f, dot));

		// Convert to angle
		float angle = std::acos(dot) * 180.f / 3.14159f;

		return angle < coneAngle;
	}
	return false;
}

void NPC2::npc2Update(sf::Time dt, const Player& player)
{

	if (active)
	{
		// Move the NPC
		Wander(dt);

		// Wrap the NPC position
		sf::Vector2f pos = npc2.getPosition();
		sf::Vector2f size = npc2.getSize();

		if (pos.x + size.x < 0)
			pos.x = WINDOW_X;
		else if (pos.x > WINDOW_X)
			pos.x = -size.x;

		if (pos.y + size.y < 0)
			pos.y = WINDOW_Y;
		else if (pos.y > WINDOW_Y)
			pos.y = -size.y;

		npc2.setPosition(pos);

		// Update vision cone
		updateVisionCone2();

		// Update sprite
		npc2_sprite.setPosition(npc2.getPosition());
		float angle = std::atan2(facingDir.y, facingDir.x) * 180.f / 3.14159f;
		npc2_sprite.setRotation(sf::degrees(angle));

		// Change vision cone color if player in view
		if (playerInVision2(player))
			visionCone.setFillColor(sf::Color(255, 0, 0, 100));
		else
			visionCone.setFillColor(sf::Color(0, 255, 0, 100));
	}
}

void NPC2::Wander(sf::Time dt)
{

	float dtSeconds = dt.asSeconds();

	// Randomly change wander orientation
	float randomChange = ((rand() % 2001) / 1000.0f - 1.0f) * wanderRate; // random between -wanderRate and +wanderRate
	wanderOrientation += randomChange * dtSeconds;

	// Current facing direction
	sf::Vector2f facing = facingDir; // normalized vector

	// Calculate the center of the wander circle
	sf::Vector2f circleCenter = npc2.getPosition() + facing * wanderOffset;

	// Calculate target on the edge of the circle
	sf::Vector2f target = circleCenter + rotateVector(facing, wanderOrientation) * wanderRadius;

	// Calculate steering (move toward target)
	sf::Vector2f desired = target - npc2.getPosition();
	float length = std::sqrt(desired.x * desired.x + desired.y * desired.y);
	if (length != 0)
		desired /= length;

	// Move NPC
	npc2.move(desired * speed * dtSeconds);

	// Update facing direction
	facingDir = desired;

}

void NPC2::setupNPC2Sprite()
{

	if (!npc2_texture.loadFromFile("ASSETS\\IMAGES\\Alien2.png"))
	{
		std::cout << "problem loading NPC sprite" << std::endl;
	}

	npc2_sprite = sf::Sprite(npc2_texture);
	npc2_sprite.setOrigin(sf::Vector2f(npc2_texture.getSize().x / 2, npc2_texture.getSize().y / 2));
	npc2_sprite.setPosition(sf::Vector2f(npc2.getPosition().x, npc2.getPosition().y));

}

