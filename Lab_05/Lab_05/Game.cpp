#include "Game.h"
#include <queue>
#include <utility>
#include <vector>
#include <limits>
#include <cmath>
#include <iostream>

Game::Game() :
	window{ sf::VideoMode{ sf::Vector2u{1050U, 1050U}, 32U }, "FlowField Pathfinding" }
{ 
	createGrid();
	//setupSprites(); 
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Game::~Game()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
		if (const auto* mouse = newEvent->getIf<sf::Event::MouseButtonPressed>())
		{
			handleMouseClick({ mouse->position.x, mouse->position.y }, mouse->button);
		}

	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Game::processKeys(const std::optional<sf::Event> t_event)
{
	const sf::Event::KeyPressed *newKeypress = t_event->getIf<sf::Event::KeyPressed>();

	if (sf::Keyboard::Key::Escape == newKeypress->code)
	{
		exitGame = true;
	}


}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Game::checkKeyboardState()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		exitGame = true;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::update(sf::Time t_deltaTime)
{
	checkKeyboardState();
	if (exitGame)
	{
		window.close();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Game::render()
{
	window.clear(sf::Color::Blue);

	//window.draw(sprite);
	drawGrid();
	drawFlowField(window);
	//drawPath(window);

	window.display();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Game::setupSprites()
{
	if (!texture.loadFromFile("ASSETS\\IMAGES\\SFML-LOGO.png"))
	{
		
		std::cout << "problem loading logo" << std::endl;
	}
	
	sprite.setTexture(texture,true);
	sprite.setPosition(sf::Vector2f{ 150.0f, 50.0f });
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
	integrationField.assign(rows, std::vector<int>(cols, 0));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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

void Game::drawFlowField(sf::RenderWindow& window)
{

	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < cols; ++x)
		{
			Tile& tile = grid[y][x];

			// Draw the base tile first
			window.draw(tile.shape);

			// Skip non-traversable tiles
			if (!tile.traversable)
				continue;

			// Draw an arrow (a line) showing flow direction
			sf::Vector2f center = tile.shape.getPosition() + sf::Vector2f(tileSize / 2.f, tileSize / 2.f);
			sf::Vector2f dir = tile.flowDir * 10.f; // make it visible

			sf::Vertex line[] = {
				{ center, sf::Color::Blue },
				{ center + dir, sf::Color::Blue }
			};

			window.draw(line, 2, sf::PrimitiveType::Lines);

			if (tile.flowDir != sf::Vector2f(0.0f, 0.0f))
			{
				// perp vectors for arrowhead
				sf::Vector2f left(-tile.flowDir.y, tile.flowDir.x);
				sf::Vector2f right(tile.flowDir.y, -tile.flowDir.x);

				float arrowSize = 3.0f; //arrowhead size
				sf::Vector2f end = center + dir;	// tip of main arrow line

				// two small offset lines forming the arrowhead
				sf::Vector2f leftTip = end - tile.flowDir * arrowSize + left * (arrowSize / 2.f);
				sf::Vector2f rightTip = end - tile.flowDir * arrowSize + right * (arrowSize / 2.f);
				                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
				sf::Vertex arrowLeft[] = { { end, sf::Color::Blue }, { leftTip, sf::Color::Blue } };
				sf::Vertex arrowRight[] = { { end, sf::Color::Blue }, { rightTip, sf::Color::Blue } };

				// draw both small lines
				window.draw(arrowLeft, 2, sf::PrimitiveType::Lines);
				window.draw(arrowRight, 2, sf::PrimitiveType::Lines);
			}
		}
	}


}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Game::drawPath(sf::RenderWindow& window)
{
	//only if both tiles exist
	if (startTile.x < 0 || endTile.x < 0)
	{
		return;
	}

	sf::Vector2i current = startTile;

	// Store visited tiles to avoid infinite loops
	std::vector<sf::Vector2i> visited;

	for (int i = 0; i < 1000; ++i) // limit to prevent infinite loops
	{
		visited.push_back(current);

		Tile& tile = grid[current.y][current.x];

		// Compute next tile based on flowDir
		sf::Vector2f dir = tile.flowDir;

		if (dir == sf::Vector2f(0.f, 0.f))
		{
			break; // No direction — stop here
		}

		// Find the next grid cell in the direction of flow
		int nextX = current.x + static_cast<int>(std::round(dir.x));
		int nextY = current.y + static_cast<int>(std::round(dir.y));

		// Break if out of bounds
		if (nextX < 0 || nextX >= cols || nextY < 0 || nextY >= rows)
		{
			break;
		}

		current = { nextX, nextY };

		// If we've reached the end tile, stop
		if (current == endTile)
		{
			visited.push_back(current);
			break;
		}
	}

	// Draw the path
	for (auto& pos : visited)
	{
		sf::CircleShape stepShape(tileSize / 4.f);

		stepShape.setFillColor(sf::Color(255, 255, 0, 180)); // Yellow with transparency
		stepShape.setOrigin(sf::Vector2f(stepShape.getRadius(), stepShape.getRadius()));
		stepShape.setPosition(sf::Vector2f(grid[pos.y][pos.x].shape.getPosition().x + tileSize / 2.f, grid[pos.y][pos.x].shape.getPosition().y + tileSize / 2.f));

		window.draw(stepShape);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Game::computeIntegrationField()
{
	const int INF = std::numeric_limits<int>::max() / 4;

	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < cols; ++x)
		{
			integrationField[y][x] = INF;
		}
	}

	std::cout << "All tiles set to INF cost (" << INF << ")\n";

	if (endTile.x < 0 || endTile.y < 0)
	{
		return;
	}

	using Node = std::pair<int, std::pair<int, int>>;

	auto cmp = [](const Node& a, const Node& b) { return a.first > b.first; };

	std::priority_queue<Node, std::vector<Node>, decltype(cmp)> pq(cmp);

	integrationField[endTile.y][endTile.x] = 0;

	pq.push({ 0, { endTile.y, endTile.x } });

	const int dirs[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} };

	while (!pq.empty())
	{
		auto cur = pq.top();
		pq.pop();

		int costSoFar = cur.first;
		int y = cur.second.first;
		int x = cur.second.second;

		if (costSoFar != integrationField[y][x]) continue;

		for (auto& d : dirs)
		{
			int ny = y + d[0];
			int nx = x + d[1];

			if (ny < 0 || ny >= rows || nx < 0 || nx >= cols)
			{
				continue;
			}

			if (!grid[ny][nx].traversable)
			{
				continue;
			}

			int newCost = costSoFar + grid[ny][nx].cost;

			if (newCost < integrationField[ny][nx])
			{
				integrationField[ny][nx] = newCost;
				pq.push({ newCost, { ny, nx } });
			}
		}
	}
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::computeFlowField()
{
	const int dirs[8][2] = { {-1, 0},  // up
							{1, 0}, // down
							{0, -1}, // left
							{0, 1},// right
							{-1, -1}, // up-left
							{-1, 1}, // up-right
							{1, -1}, // down-left
							{1, 1} };// down-right

	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < cols; ++x)
		{
			Tile& tile = grid[y][x];

			if (!tile.traversable)
			{
				tile.flowDir = { 0.f, 0.f };
				continue;
			}

			int bestCost = integrationField[y][x];
			sf::Vector2f bestDir = { 0.f, 0.f };

			// look at all neighbors, pick one with smallest integration cost
			for (auto& d : dirs)
			{
				int ny = y + d[0];
				int nx = x + d[1];

				if (ny < 0 || ny >= rows || nx < 0 || nx >= cols)
				{
					continue;
				}

				if (integrationField[ny][nx] < bestCost)
				{
					bestCost = integrationField[ny][nx];
					bestDir = sf::Vector2f((float)d[1], (float)d[0]); // (x,y) swap
				}
			}

			// normalize so the arrow length is consistent
			float length = std::sqrt(bestDir.x * bestDir.x + bestDir.y * bestDir.y);

			if (length > 0)
			{
				tile.flowDir = bestDir / length;
			}
			else
			{
				tile.flowDir = { 0.f, 0.f };
			}

		}
	}

	std::cout << "Flow field computed.\n";
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Game::handleMouseClick(sf::Vector2i mousePos, sf::Mouse::Button button)
{

	int gridX = mousePos.x / tileSize;
	int gridY = mousePos.y / tileSize;

	// within gridbounds check
	if (gridX < 0 || gridX >= cols || gridY < 0 || gridY >= rows)
	{
		return;
	}


	if (button == sf::Mouse::Button::Middle)
	{
		// Get a reference to the tile that was clicked
		Tile& clickedTile = grid[gridY][gridX];

		// Toggle the tile’s walkable state:
		clickedTile.traversable = !clickedTile.traversable;

		// Set the movement cost:
		if (clickedTile.traversable)		// - Normal tiles have a cost of 1.
		{
			clickedTile.cost = 1;
		}
		else
		{
			clickedTile.cost = 9999;	// - Obstacles have a huge cost (effectively unwalkable).
		}

		// Change the tile color:
		if (clickedTile.traversable)
		{
			clickedTile.shape.setFillColor(sf::Color::White);		// - White for walkable.
		}
		else
		{
			clickedTile.shape.setFillColor(sf::Color(60, 60, 60)); // // - Dark gray for obstacle.
		}

		// If new obstacle happens to cover the start or goal tile,
		// remove those markers so they don’t sit on top of walls.
		if (!clickedTile.traversable)
		{
			if (startTile.x == gridX && startTile.y == gridY)
			{
				startTile = { -1, -1 };
			}

			if (endTile.x == gridX && endTile.y == gridY)
			{
				endTile = { -1, -1 };
			}

		}

		// If a goal tile exists (red tile already placed),
		// recalculate the integration and flow fields
		if (endTile.x >= 0)
		{
			computeIntegrationField();
			computeFlowField();
		}

		return;
	}

	if (button == sf::Mouse::Button::Left)
	{
		// reset previous start tile if it still exists and is traversable
		if (startTile.x >= 0)
		{
			computeIntegrationField();
			computeFlowField();

			auto& prev = grid[startTile.y][startTile.x];
			prev.shape.setFillColor(prev.traversable ? sf::Color::White : sf::Color(60, 60, 60));
		}
		// set new start
		startTile = { gridX, gridY };
		grid[gridY][gridX].shape.setFillColor(sf::Color::Green);



		return;

	}
	
	if (button == sf::Mouse::Button::Right)
	{
		// reset previous end tile color
		if (endTile.x >= 0)
		{
			auto& prev = grid[endTile.y][endTile.x];
			prev.shape.setFillColor(prev.traversable ? sf::Color::White : sf::Color(60, 60, 60));
		}
		endTile = { gridX, gridY };
		grid[gridY][gridX].shape.setFillColor(sf::Color::Red);

		// compute integration and flow fields now that we have a goal
		computeIntegrationField();
		computeFlowField();
		std::cout << "Goal tile at (" << endTile.x << ", " << endTile.y << ") set to cost 0\n";
		return;
	}

}
