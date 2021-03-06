/*
 * THIS WAS CREATED USING VISUAL STUDIO COMMUNITY EDITION - V 16.2.5   
 */

/*
 * This is the main point of entry for the program and handles
 *		- Window creation.
 *		- Polling window events.
 *		- The game loop.
 *		- Creation and updating and deletion of all screen obejcts.
 *
 * Original @author Paul Robertson.
 *
 * Updated by @author D. Green.
 *
 * © D. Green. 2020.
 */

 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES.
#include <iostream>
#include "Screen.h"
#include "Menu.h"
#include "Level.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include "Pause.h"
#include "Splash.h"
#include "HowToPlay.h"
#include "Options.h"
#include "YouDied.h"
#include "GameOver.h"
#include "YouEscaped.h"
#include "Credits.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void windowProcess(sf::RenderWindow* window, Input* in)
{
	// Handle window events.
	sf::Event event;
	while (window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::Resized:
			window->setView(sf::View(sf::FloatRect(0.f, 0.f, (float)event.size.width, (float)event.size.height)));
			break;
		case sf::Event::KeyPressed:
			// update input class
			in->setKeyDown(event.key.code);
			break;
		case sf::Event::KeyReleased:
			//update input class
			in->setKeyUp(event.key.code);
			break;
		case sf::Event::MouseMoved:
			//update input class
			in->setMousePosition(event.mouseMove.x, event.mouseMove.y);
			break;
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				//update input class
				in->setMouseLDown(true);
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				in->setMouseRDown(true);
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				//update input class
				in->setMouseLDown(false);
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				in->setMouseRDown(false);
			}
			break;
		default:
			// don't handle other events
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	//Create the window // Initialise input and level objects.
	sf::RenderWindow window(sf::VideoMode(960, 512), "ESCAPE FROM CRASTERS KEEP!");
	Input input;
	GameState gameState;
	AudioManager audioManager;

	// Create an extra loop here in order to start an entirely new game, it will create a new game object. make sure to call the destructors on all the old objects.

	Splash* splash =  new Splash(&window, &input, &gameState, &audioManager);

	while (window.isOpen())
	{
		// Moved these all onto the heap as i begun to exceed stack memory.
		Menu* menu = new Menu(&window, &input, &gameState, &audioManager);
		HowToPlay* howToPlay = new HowToPlay(&window, &input, &gameState, &audioManager);
		Options* options =  new Options(&window, &input, &gameState, &audioManager);
		Level* level = new Level(&window, &input, &gameState, &audioManager);
		YouDied* youDied = new YouDied(&window, &input, &gameState, &audioManager);
		GameOver* gameover = new GameOver(&window, &input, &gameState, &audioManager);
		YouEscaped* youEscaped = new YouEscaped(&window, &input, &gameState, &audioManager);
		Pause* pause = new Pause(&window, &input, &gameState, &audioManager);
		Credits* credits = new Credits(&window, &input, &gameState, &audioManager);

		// Initialise objects for delta time
		sf::Clock clock;
		float deltaTime;

		// Game Loop, check for death game over or escaped game over.
		while (!gameover->getGameOver() && !level->getGameOver() && window.isOpen())
		{
			//Process window events
			windowProcess(&window, &input);

			// Calculate delta time. How much time has passed 
			// since it was last calculated (in seconds) and restart the clock.
			deltaTime = clock.restart().asSeconds();

			switch (gameState.getCurrentState())
			{
			case(State::SPLASH):
				splash->handleInput(deltaTime);
				splash->update(deltaTime);
				splash->render();
				break;
			case(State::MENU):
				menu->handleInput(deltaTime);
				menu->update(deltaTime);
				menu->render();
				break;
			case(State::HOW_TO_PLAY):
				howToPlay->handleInput(deltaTime);
				howToPlay->update(deltaTime);
				howToPlay->render();
				break;
			case(State::OPTIONS):
				options->handleInput(deltaTime);
				options->update(deltaTime);
				options->render();
				break;
			case(State::LEVEL):
				level->handleInput(deltaTime);
				level->update(deltaTime);
				level->render();
				break;
			case(State::YOU_DIED):
				youDied->handleInput(deltaTime);
				youDied->update(deltaTime);
				youDied->render();
				break;
			case(State::GAMEOVER):
				gameover->handleInput(deltaTime);
				gameover->update(deltaTime);
				gameover->render();
				break;
			case(State::YOU_ESCAPED):
				youEscaped->handleInput(deltaTime);
				youEscaped->update(deltaTime);
				youEscaped->render();
				break;
			case(State::PAUSE):
				pause->handleInput(deltaTime);
				pause->update(deltaTime);
				pause->render();
				break;
			case(State::CREDITS):
				credits->handleInput(deltaTime);
				credits->update(deltaTime);
				credits->render();
				break;
			}
		}

		// If the game is over, clean up everything so we can create new ones on the next iteration of the outer while loop.
		if (gameover->getGameOver() || level->getGameOver())
		{
			delete menu;
			menu = nullptr;

			delete howToPlay;
			howToPlay = nullptr;

			delete options;
			options = nullptr;

			delete level;
			level = nullptr;

			delete youDied;
			youDied = nullptr;

			delete gameover;
			gameover = nullptr;

			delete youEscaped;
			youEscaped = nullptr;

			delete pause;
			pause = nullptr;
		}
	}	
}