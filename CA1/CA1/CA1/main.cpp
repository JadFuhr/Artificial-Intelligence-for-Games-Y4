/**
 * @file main.cpp
 * @brief Entry point for The Fourth Protocol game
 * @author Jad Fuhr 
 * @date 05/12/2025
 *
 * This file contains the main function that initializes and runs the game.
 */

#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 

#include <iostream>
#include "Game.h"

 /**
  * @brief Main entry point of the application
  *
  * Creates a Game instance and runs the main game loop.
  *
  * @return EXIT_SUCCESS on successful execution
  */
int main()
{
	Game game;
	game.run();

	return EXIT_SUCCESS;
}