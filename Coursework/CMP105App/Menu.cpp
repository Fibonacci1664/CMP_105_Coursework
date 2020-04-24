/*
 * This class controls
 *		- Creating a bg image for the Menu screen.
 *		- Checking button collisions.
 *		- Changing textures when button collisions detected.
 *		- Fading in/out to/from menu.
 *
 * Original @author D. Green.
 *
 * © D. Green. 2020.
 */

 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES.
#include "Menu.h"
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR/S & DESTRUCTOR.
Menu::Menu(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud) : Screen(hwnd, in, gs, aud)
{
	mousePos = sf::Vector2f(input->getMouseX(), input->getMouseY());
	fadedIn = false;
	howToPlayClicked = false;
	optionsClicked = false;
	newGameClicked = false;
	quitClicked = false;

	initAudio();
	initMenuBackground();
	initHowToPlayButton();
	initOptionsButton();
	initNewGameButton();
	initQuitButton();
	initTransFadeRect();
}

Menu::~Menu()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS.
void Menu::handleInput(float dt)
{
	checkHowToPlayButtonCollisions();
	checkOptionsButtonCollisions();
	checkNewGameButtonCollisions();
	checkQuitButtonCollisions();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::update(float dt)
{
	mousePos = sf::Vector2f(input->getMouseX(), input->getMouseY());
	customCursor.setPosition(sf::Vector2f(input->getMouseX(), input->getMouseY()));

	if (audio->getMusic()->getStatus() == sf::SoundSource::Stopped)
	{
		// If we're still in menu then play the menu ambience.
		if (gameState->getCurrentState() == State::MENU)
		{
			audio->playMusicbyName("ambience");
		}
		else	// Otherwise we must have switched states to a new game.
		{
			audio->stopAllMusic();
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::render()
{
	beginDraw();

	if (!fadedIn)
	{
		float decr = 255;

		while (decr > 0)
		{
			decr -= 0.07f;

			transFade.setFillColor(sf::Color(0, 0, 0, decr));
			window->draw(menuBg);
			window->draw(howToPlayButton);
			window->draw(optionsButton);
			window->draw(newGameButton);
			window->draw(quitButton);
			window->draw(transFade);
			endDraw();

			if (decr < 1)
			{
				fadedIn = true;
			}
		}
	}

	// Draw all things one last time to ensure they are on top and NOT the trans layer.
	window->draw(menuBg);
	window->draw(howToPlayButton);
	window->draw(optionsButton);
	window->draw(newGameButton);
	window->draw(quitButton);
	window->draw(customCursor);

	endDraw();
	//Sleep(500);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::beginDraw()
{
	window->clear(sf::Color(0, 0, 0));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::endDraw()
{
	window->display();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::initAudio()
{
	audio->addSound("sfx/menu/sword.ogg", "swoosh");
	audio->getSound("swoosh")->setVolume(40);
	audio->addMusic("sfx/menu/menu_ambience.ogg", "ambience");
	audio->getMusic()->setVolume(50);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::initMenuBackground()
{
	if (!menuBgTexture.loadFromFile("gfx/screens/crasters_keep_birds_final.png"))
	{
		std::cerr << "Sorry could not load menu background image!\n";
	}

	menuBg.setSize(sf::Vector2f(960, 540));
	menuBg.setTexture(&menuBgTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Load in the textures for the how to play button, normal, hover, clicked.
void Menu::initHowToPlayButton()
{
	if (!howToPlayButtonTexture.loadFromFile("gfx/buttons/normal/how_to_play_button.png"))
	{
		std::cerr << "Sorry could not load how to play button image!\n";
	}

	if (!howToPlayButtonHoverTexture.loadFromFile("gfx/buttons/hover/htp_hover.png"))
	{
		std::cerr << "Sorry could not load how to play button hover image!\n";
	}

	if (!howToPlayButtonClickedTexture.loadFromFile("gfx/buttons/clicked/htp_clicked.png"))
	{
		std::cerr << "Sorry could not load how to play button clicked image!\n";
	}

	howToPlayButton.setSize(sf::Vector2f(175.2, 31.8f));
	howToPlayButton.setPosition(sf::Vector2f(80, 210));
	howToPlayButton.setTexture(&howToPlayButtonTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Load in the textures for the new game button, normal, hover, clicked.
void Menu::initNewGameButton()
{
	if (!newGameButtonTexture.loadFromFile("gfx/buttons/normal/ng_continue_button.png"))
	{
		std::cerr << "Sorry could not load new game button image!\n";
	}

	if (!newGameButtonHoverTexture.loadFromFile("gfx/buttons/hover/ng_continue_hover.png"))
	{
		std::cerr << "Sorry could not load new game button hover image!\n";
	}

	if (!newGameButtonClickedTexture.loadFromFile("gfx/buttons/clicked/ng_continue_clicked.png"))
	{
		std::cerr << "Sorry could not load new game button clicked image!\n";
	}

	newGameButton.setSize(sf::Vector2f(260.4, 31.8f));
	newGameButton.setPosition(sf::Vector2f(65, 170));
	newGameButton.setTexture(&newGameButtonTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Load in the textures for the options button, normal, hover, clicked.
void Menu::initOptionsButton()
{
	if (!optinosButtonTexture.loadFromFile("gfx/buttons/normal/options_button.png"))
	{
		std::cerr << "Sorry could not load options button image!\n";
	}

	if (!optionsButtonHoverTexture.loadFromFile("gfx/buttons/hover/o_hover.png"))
	{
		std::cerr << "Sorry could not load options button hover image!\n";
	}

	if (!optionsButtonClickedTexture.loadFromFile("gfx/buttons/clicked/o_clicked.png"))
	{
		std::cerr << "Sorry could not load options button clicked image!\n";
	}

	optionsButton.setSize(sf::Vector2f(112.8f, 31.8f));
	optionsButton.setPosition(sf::Vector2f(95, 250));
	optionsButton.setTexture(&optinosButtonTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Load in the textures for the quit button, normal, hover, clicked.
void Menu::initQuitButton()
{
	if (!quitButtonTexture.loadFromFile("gfx/buttons/normal/quit_button.png"))
	{
		std::cerr << "Sorry could not load quit button image!\n";
	}

	if (!quitButtonHoverTexture.loadFromFile("gfx/buttons/hover/q_hover.png"))
	{
		std::cerr << "Sorry could not load quit button hover image!\n";
	}

	if (!quitButtonClickedTexture.loadFromFile("gfx/buttons/clicked/q_clicked.png"))
	{
		std::cerr << "Sorry could not load quit button clicked image!\n";
	}

	quitButton.setSize(sf::Vector2f(78.6f, 31.8f));
	quitButton.setPosition(sf::Vector2f(110, 290));
	quitButton.setTexture(&quitButtonTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Create a transparent panel, used for fading effects.
void Menu::initTransFadeRect()
{
	transFade.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::checkHowToPlayButtonCollisions()
{
	// If the cursor is hovering over the button.
	if (checkMouseCollisions(&howToPlayButton, mousePos))
	{
		//std::cout << "Hover collision detected with the how to play button!\n";
		howToPlayButton.setTexture(&howToPlayButtonHoverTexture);

		// If the cursor is hovering AND we click the left mouse button.
		if (checkMouseCollisions(&howToPlayButton, mousePos) && input->isMouseLDown())
		{
			//std::cout << "Clicked on the how to play button!\n";
			howToPlayButton.setTexture(&howToPlayButtonClickedTexture);
			howToPlayClicked = true;
		}
	}
	else		// Otherwise just set the normal button texture.
	{
		howToPlayButton.setTexture(&howToPlayButtonTexture);
	}

	// If weve clicked the button and released the left mouse button the switch states.
	if (howToPlayClicked && !input->isMouseLDown())
	{
		audio->playSoundbyName("swoosh");
		howToPlayClicked = false;
		setGameState(State::HOW_TO_PLAY);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::checkOptionsButtonCollisions()
{
	if (checkMouseCollisions(&optionsButton, mousePos))
	{
		//std::cout << "Hover collision detected with the how to play button!\n";
		optionsButton.setTexture(&optionsButtonHoverTexture);

		if (checkMouseCollisions(&optionsButton, mousePos) && input->isMouseLDown())
		{
			//std::cout << "Clicked on the how to play button!\n";
			optionsButton.setTexture(&optionsButtonClickedTexture);
			optionsClicked = true;
		}
	}
	else
	{
		optionsButton.setTexture(&optinosButtonTexture);
	}

	if (optionsClicked && !input->isMouseLDown())
	{
		audio->playSoundbyName("swoosh");
		optionsClicked = false;
		setGameState(State::OPTIONS);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::checkNewGameButtonCollisions()
{
	if (checkMouseCollisions(&newGameButton, mousePos))
	{
		//std::cout << "Collision detected with the new game button!\n";

		newGameButton.setTexture(&newGameButtonHoverTexture);

		if (checkMouseCollisions(&newGameButton, mousePos) && input->isMouseLDown())
		{
			//std::cout << "Clicked on the new game button!\n";
			newGameButton.setTexture(&newGameButtonClickedTexture);
			newGameClicked = true;	
		}
	}
	else
	{
		newGameButton.setTexture(&newGameButtonTexture);
	}

	if (newGameClicked && !input->isMouseLDown())
	{
		audio->playSoundbyName("swoosh");
		audio->stopAllMusic();
		newGameClicked = false;
		setGameState(State::LEVEL);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::checkQuitButtonCollisions()
{
	if (checkMouseCollisions(&quitButton, mousePos))
	{
		//std::cout << "Collision detected with the quit button!\n";

		quitButton.setTexture(&quitButtonHoverTexture);

		if (checkMouseCollisions(&quitButton, mousePos) && input->isMouseLDown())
		{
			//std::cout << "Clicked on the quit button!\n";
			quitButton.setTexture(&quitButtonClickedTexture);
			quitClicked = true;
		}
	}
	else
	{
		quitButton.setTexture(&quitButtonTexture);
	}

	if (quitClicked && !input->isMouseLDown())
	{
		audio->playSoundbyName("swoosh");
		quitClicked = false;
		setGameState(State::CREDITS);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////