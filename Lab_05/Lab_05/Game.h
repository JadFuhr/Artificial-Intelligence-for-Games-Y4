#ifndef GAME_HPP
#define GAME_HPP
#pragma warning( push )
#pragma warning( disable : 4275 )

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

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
	void setupSprites();

	sf::RenderWindow window; 
	sf::Font font;	
	sf::Texture texture;
	sf::Sprite sprite{ texture }; 
	
	bool exitGame = false; 
};

#pragma warning( pop ) 
#endif 

