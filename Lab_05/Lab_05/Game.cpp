#include "Game.h"
#include <iostream>

Game::Game() :
	window{ sf::VideoMode{ sf::Vector2u{1050U, 1050U}, 32U }, "FlowField Pathfinding" }
{ 
	createGrid();
	//setupSprites(); 
}

Game::~Game()
{
}

void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); 
	while (window.isOpen())
	{
		processEvents(); 
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); 
			update(timePerFrame); 
		}
		render(); 
	}
}

void Game::processEvents()
{
	
	while (const std::optional newEvent = window.pollEvent())
	{
		if ( newEvent->is<sf::Event::Closed>()) 
		{
			exitGame = true;
		}
		if (newEvent->is<sf::Event::KeyPressed>()) 
		{
			processKeys(newEvent);
		}
		if (const auto* mouseEvent = newEvent->getIf<sf::Event::MouseButtonPressed>())
		{
			sf::Vector2i mousePos(mouseEvent->position.x, mouseEvent->position.y);

			bool isLeft = (mouseEvent->button == sf::Mouse::Button::Left);

			handleMouseClick(mousePos, isLeft);
		}

	}
}

void Game::processKeys(const std::optional<sf::Event> t_event)
{
	const sf::Event::KeyPressed *newKeypress = t_event->getIf<sf::Event::KeyPressed>();

	if (sf::Keyboard::Key::Escape == newKeypress->code)
	{
		exitGame = true;
	}


}

void Game::checkKeyboardState()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		exitGame = true;
	}
}

void Game::update(sf::Time t_deltaTime)
{
	checkKeyboardState();
	if (exitGame)
	{
		window.close();
	}
}

void Game::render()
{
	window.clear(sf::Color::Blue);

	//window.draw(sprite);
	drawGrid();

	window.display();
}

void Game::setupSprites()
{
	if (!texture.loadFromFile("ASSETS\\IMAGES\\SFML-LOGO.png"))
	{
		
		std::cout << "problem loading logo" << std::endl;
	}
	
	sprite.setTexture(texture,true);
	sprite.setPosition(sf::Vector2f{ 150.0f, 50.0f });
}

void Game::createGrid()
{

	grid.reserve(rows);

	for (int y = 0; y < rows; ++y)
	{
		std::vector<Tile> row;
		row.reserve(cols);
		for (int x = 0; x < cols; ++x)
		{
			float posX = x * tileSize;
			float posY = y * tileSize;
			row.emplace_back(posX, posY, tileSize);
		}
		grid.push_back(std::move(row));
	}

}

void Game::drawGrid()
{

	for (auto& row : grid)
	{
		for (auto& tile : row)
		{
			window.draw(tile.shape);
		}
	}

}

void Game::handleMouseClick(sf::Vector2i mousePos, bool isLeftClick)
{

	int gridX = mousePos.x / tileSize;
	int gridY = mousePos.y / tileSize;

	// Ensure click is within grid bounds
	if (gridX < 0 || gridX >= cols || gridY < 0 || gridY >= rows)
		return;

	if (isLeftClick)
	{
		// Reset previous start tile
		if (startTile.x >= 0)
		{
			grid[startTile.y][startTile.x].shape.setFillColor(sf::Color::White);
		}


		startTile = { gridX, gridY };
		grid[gridY][gridX].shape.setFillColor(sf::Color::Green);
	}
	else
	{
		// Reset previous goal tile
		if (endTile.x >= 0)
		{
			grid[endTile.y][endTile.x].shape.setFillColor(sf::Color::White);
		}


		endTile = { gridX, gridY };
		grid[gridY][gridX].shape.setFillColor(sf::Color::Red);
	}

}
