/// <summary>
/// author Jad Fuhr 
/// </summary>


#include "Game.h"
#include <iostream>


/// default constructor
/// setup the window properties
/// load and setup the texts
/// load and setup the images
/// load and setup the sounds

Game::Game() :
	m_window{ sf::VideoMode{ sf::Vector2u{800U, 600U}, 32U }, "SFML Game 3.0" },
	m_DELETEexitGame{false} //when true game will exit
{
	setupTexts(); // load font 
	setupSprites(); // load texture
	setupAudio(); // load sounds
	setupPlayer();// setup player
	setupNPC();// setup npc
	
}


/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here

Game::~Game()
{
}


/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames

void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}

/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here

void Game::processEvents()
{
	
	while (const std::optional newEvent = m_window.pollEvent())
	{
		if ( newEvent->is<sf::Event::Closed>()) // close window message 
		{
			m_DELETEexitGame = true;
		}
		if (newEvent->is<sf::Event::KeyPressed>()) //user pressed a key
		{
			processKeys(newEvent);
		}
	}
}



/// deal with key presses from the user

/// <param name="t_event">key press event</param>
void Game::processKeys(const std::optional<sf::Event> t_event)
{
	const sf::Event::KeyPressed *newKeypress = t_event->getIf<sf::Event::KeyPressed>();
	if (sf::Keyboard::Key::Escape == newKeypress->code)
	{
		m_DELETEexitGame = true; 
	}
}


/// Check if any keys are currently pressed

void Game::checkKeyboardState()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		m_DELETEexitGame = true; 
	}
}


/// Update the game world
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	checkKeyboardState();
	if (m_DELETEexitGame)
	{
		m_window.close();
	}
}


/// draw the frame and then switch buffers

void Game::render()
{
	m_window.clear();


	m_window.draw(m_DELETEwelcomeMessage);
	m_window.draw(m_player);
	m_window.draw(m_npc);

	m_window.display();
}


/// load the font and setup the text message for screen

void Game::setupTexts()
{
	if (!m_jerseyFont.openFromFile("ASSETS\\FONTS\\Jersey20-Regular.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	m_DELETEwelcomeMessage.setFont(m_jerseyFont);
	m_DELETEwelcomeMessage.setString("Basic Movement");
	m_DELETEwelcomeMessage.setOrigin(sf::Vector2f(30, 6));
	m_DELETEwelcomeMessage.setPosition(sf::Vector2f{ 240.0f, 20.0f });
	m_DELETEwelcomeMessage.setCharacterSize(60U);
	m_DELETEwelcomeMessage.setOutlineColor(sf::Color::White);
	m_DELETEwelcomeMessage.setFillColor(sf::Color::Black);
	m_DELETEwelcomeMessage.setOutlineThickness(2.0f);

}


/// load the texture and setup the sprite for the logo

void Game::setupSprites()
{
	//if (!m_DELETElogoTexture.loadFromFile("ASSETS\\IMAGES\\SFML-LOGO.png"))
	//{
	//	// simple error message if previous call fails
	//	std::cout << "problem loading logo" << std::endl;
	//}
	//m_DELETElogoSprite.setTexture(m_DELETElogoTexture,true);// to reset the dimensions of texture
	//m_DELETElogoSprite.setPosition(sf::Vector2f{ 150.0f, 50.0f });


}

void Game::setupPlayer()
{
	m_player.setSize(sf::Vector2f(50.0f, 50.0f));
	m_player.setOrigin(sf::Vector2f(25.0f, 25.0f));
	m_player.setFillColor(sf::Color::Black);
	m_player.setOutlineColor(sf::Color::White);
	m_player.setOutlineThickness(2);
	m_player.setPosition(sf::Vector2f(400.0f, 300.0f));

}

void Game::setupNPC()
{

	m_npc.setSize(sf::Vector2f(20.0f, 20.0f));
	m_npc.setOrigin(sf::Vector2f(10.0f, 10.0f));
	m_npc.setFillColor(sf::Color::White);
	m_npc.setPosition(sf::Vector2f(600.0f, 250.0f));

}


/// load sound file and assign buffers

void Game::setupAudio()
{
	if (!m_DELETEsoundBuffer.loadFromFile("ASSETS\\AUDIO\\beep.wav"))
	{
		std::cout << "Error loading beep sound" << std::endl;
	}
	m_DELETEsound.play(); // test sound
}


