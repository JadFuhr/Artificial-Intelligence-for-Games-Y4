//Author: Jad Fuhr
//Student Number: C00290965
 
 
#include "Game.h"
#include <iostream>

Game::Game() :
	window{ sf::VideoMode{ sf::Vector2u{WINDOW_X, WINDOW_Y}, 32U }, "Basic Movement" },player(50.0f),npc1(25.0f),npc2(25.0f)
{ 
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
	}
}

void Game::processKeys(const std::optional<sf::Event> t_event)
{
	const sf::Event::KeyPressed *newKeypress = t_event->getIf<sf::Event::KeyPressed>();
	if (sf::Keyboard::Key::Escape == newKeypress->code)
	{
		exitGame = true;
	}

	if (sf::Keyboard::Key::Num1 == newKeypress->code)
	{
		npc1.toggleActive();
	}
	if (sf::Keyboard::Key::Num2 == newKeypress->code)
	{
		npc2.toggleActive2();
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

	player.handleInput(t_deltaTime);
	player.playerUpdate(t_deltaTime);
	npc1.npcUpdate(t_deltaTime, player);
	npc2.npc2Update(t_deltaTime, player);
	if (exitGame)
	{
		window.close();
	}
}


void Game::render()
{
	window.clear(sf::Color::Black);

	player.playerRender(window);
	npc1.npcRender(window);
	npc2.npc2Render(window);
	window.display();
}



