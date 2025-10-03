#include "NPC.h"


NPC::NPC(float size)
{
	npc1.setSize(sf::Vector2f(size, size));
	npc1.setFillColor(sf::Color::White);
	npc1.setPosition(sf::Vector2f(700.0f, 600.0f));
	npc1.setOrigin(sf::Vector2f(size / 2.0f, size / 2.0f));


	setupNPCSprite();
	setUpVisionCone();
	//setUpText();
}

void NPC::npcUpdate(sf::Time dt, const Player& player)
{

	sf::Vector2f pos = npc1.getPosition();
	sf::Vector2f size = npc1.getSize();

	if (active)
	{
		Seek(dt, player);
		updateVisionCone();

		npc1_sprite.setPosition(pos);
		npc1_sprite.rotate(sf::degrees(5));

		if (playerInVision(player))
		{
			visionCone.setFillColor(sf::Color(255, 0, 0, 100));
		}
		else
		{
			visionCone.setFillColor(sf::Color(0, 255, 0, 100));
		}
	}


	if (pos.x + size.x < 0)
	{
		pos.x = WINDOW_X;
	}
	else if (pos.x > WINDOW_X)
	{
		pos.x = -size.x;
	}

	// vertical wrapping 

	if (pos.y + size.y < 0)
	{
		pos.y = WINDOW_Y;
	}
	else if (pos.y > WINDOW_Y)
	{
		pos.y = -size.y;
	}


	sf::Vector2f npcPos = npc1.getPosition();

}


void NPC::npcRender(sf::RenderWindow& window)
{
	if (visible)
	{
		window.draw(npc1);
		window.draw(npc1_sprite);
		window.draw(visionCone);
	}

}

void NPC::Seek(sf::Time dt, const Player& player)
{

	sf::Vector2f playerPos = player.getPosition();
	sf::Vector2f npcPos = npc1.getPosition();

	sf::Vector2f moveDir = playerPos - npcPos;
	float length = std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);


	if (length != 0)
	{
		moveDir /= length;
		npc1.move(moveDir * speed * dt.asSeconds());
		facingDir = moveDir;
	}

}

void NPC::setupNPCSprite()
{

	if (!npc1_texture.loadFromFile("ASSETS\\IMAGES\\Alien.png"))
	{
		std::cout << "problem loading NPC sprite" << std::endl;
	}

	npc1_sprite = sf::Sprite(npc1_texture);
	npc1_sprite.setOrigin(sf::Vector2f(npc1_texture.getSize().x / 2, npc1_texture.getSize().y / 2));
	npc1_sprite.setPosition(sf::Vector2f(npc1.getPosition().x, npc1.getPosition().y));


}

void NPC::setUpVisionCone()
{

	visionCone.setPointCount(3);
	visionCone.setPoint(0, sf::Vector2f(0.f, 0.f));
	visionCone.setPoint(1, sf::Vector2f(coneLength, -coneLength / 2));
	visionCone.setPoint(2, sf::Vector2f(coneLength, coneLength / 2));
	visionCone.setFillColor(sf::Color(0,255,0,100));
	visionCone.setOrigin(sf::Vector2f(0.f, 0.f));
	visionCone.setPosition(npc1.getPosition());


}


void NPC::updateVisionCone()
{
	if (active)
	{
		visionCone.setPosition(npc1.getPosition());

		float angle = std::atan2(facingDir.y, facingDir.x) * 180.f / 3.14159f;
		visionCone.setRotation(sf::degrees(angle));
	}


}

bool NPC::playerInVision(const Player& player)
{
	if (active)
	{
		sf::Vector2f npcPos = npc1.getPosition();
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

void NPC::toggleActive()
{
	active = !active;
}

void NPC::toggleVisible()
{
	visible = !visible;
}




