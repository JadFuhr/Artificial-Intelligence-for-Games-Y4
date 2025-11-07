#ifndef GAME_HPP
#define GAME_HPP
#pragma warning( push )
#pragma warning( disable : 4275 )

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Tiles.h"


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

	void createGrid();
	void drawGrid();
	void drawFlowField(sf::RenderWindow& window);
	void drawPath(sf::RenderWindow& window);

	void computeIntegrationField();
	void computeFlowField();
	void handleMouseClick(sf::Vector2i mousePos, sf::Mouse::Button button);

	sf::RenderWindow window; 
	sf::Font font;	
	sf::Texture texture;
	sf::Sprite sprite{ texture }; 
	
	// grid data
	static const int rows = 50;
	static const int cols = 50;
	static constexpr float tileSize = 21.0f; // adjust for window size
	std::vector<std::vector<Tile>> grid;

	std::vector<std::vector<int>> integrationField;
	const int INF = std::numeric_limits<int>::max() / 4;		// gives largest num an int can hold,divided by 4 just for convenience sake (defines large number to represent infinity )

	// mouse click variables
	sf::Vector2i startTile = { -1,-1 };
	sf::Vector2i endTile = { -1, -1 };

	bool exitGame = false; 
};

#pragma warning( pop ) 
#endif 

