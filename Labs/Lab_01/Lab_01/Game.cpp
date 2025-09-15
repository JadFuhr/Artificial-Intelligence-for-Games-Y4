//Author: Jad Fuhr
//Student Number: C00290965
 
 
#include "Game.h"
#include "Player.h"
#include "NPC.h"
#include <iostream>

Game::Game() :
	window{ sf::VideoMode{ sf::Vector2u{1020U, 720U}, 32U }, "Basic Movement" },player(50.0f),npc(25.0f)
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

	player.handleInput();

	if (exitGame)
	{
		window.close();
	}
}


void Game::render()
{
	window.clear(sf::Color::Black);

	player.playerRender(window);
	npc.npcRender(window);

	window.display();
}

void Game::setupSprites()
{
	//if (!texture.loadFromFile("ASSETS\\IMAGES\\SFML-LOGO.png"))
	//{
	//	
	//	std::cout << "problem loading logo" << std::endl;
	//}
	//
	//sprite.setTexture(texture,true);
	//sprite.setPosition(sf::Vector2f{ 150.0f, 50.0f });
}