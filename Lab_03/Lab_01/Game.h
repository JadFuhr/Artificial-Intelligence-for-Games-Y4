#ifndef GAME_HPP
#define GAME_HPP
#pragma warning( push )
#pragma warning( disable : 4275 )
// ignore this warning
// C:\SFML - 3.0.0\include\SFML\System\Exception.hpp(41, 47) : 
// warning C4275 : non dll - interface class 'std::runtime_error' used as base for dll - interface class 'sf::Exception'

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "NPC.h"
#include "NPC2.h"
#include "Swarm.h"
#include "Constants.h"

class Game
{
public:
	Game();
	~Game();
	void run();

private:
	void processEvents();
	void processKeys(const std::optional<sf::Event> t_event);
	void checkKeyboardState();
	void update(sf::Time t_deltaTime);
	void render();

	Player player;
	NPC npc1;
	NPC2 npc2;

	sf::RenderWindow window; 
	sf::Font font;	
	sf::Texture texture;
	sf::Sprite sprite{ texture }; 
	
	bool exitGame = false; 
};

#pragma warning( pop ) 
#endif 

