/*
 * This class controls
 *		- Creating a bg image for the Options screen.
 *		- Checking button collisions.
 *		- Checking for check box collisions.
 *		- Changing textures when collisions detected.
 *
 * Original @author D. Green.
 *
 * ? D. Green. 2020.
 */

 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES.
#include "Options.h"
#include "Level.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR/S & DESTRUCTOR.
Options::Options(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud) : Screen(hwnd, in, gs, aud)
{
	backButtonClicked = false;

	initAudio();
	initOptionsBg();
	initBackButton();
	initCheckBoxTextures();
	initMuteMusicCheckBox();
	initMuteSFXCheckBox();
	initGodModeCheckBox();

	musicChecked = false;
}

Options::~Options()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS.
void Options::handleInput(float dt)
{
	checkBackButtonCollisions();
	checkMuteMusicBoxCollisions();
	checkMuteSFXBoxCollisions();
	checkGodModeBoxCollisions();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Options::update(float dt)
{
	//std::cout << "Options update being called!\n";

	mousePos = sf::Vector2f(input->getMouseX(), input->getMouseY());
	customCursor.setPosition(sf::Vector2f(input->getMouseX(), input->getMouseY()));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Options::render()
{
	beginDraw();
	window->draw(optionsBg);
	window->draw(backButton);
	window->draw(muteMusicCheckBox);
	window->draw(muteSFXCheckBox);
	window->draw(godModeCheckBox);
	window->draw(customCursor);
	endDraw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Options::beginDraw()
{
	//window->clear(sf::Color(0, 255, 0));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Options::endDraw()
{
	window->display();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Options::initAudio()
{
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Options::initOptionsBg()
{
	if (!optionsBgTexture.loadFromFile("gfx/screens/options_birds.png"))
	{
		std::cerr << "Sorry could not how to play background image!\n";
	}

	optionsBg.setSize(sf::Vector2f(960, 540));
	optionsBg.setTexture(&optionsBgTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Load in back button textures, normal, hover, clicked.
void Options::initBackButton()
{
	if (!backButtonTexture.loadFromFile("gfx/buttons/normal/back_button.png"))
	{
		std::cerr << "Sorry could not load back button image!\n";
	}

	if (!backButtonHoverTexture.loadFromFile("gfx/buttons/hover/b_hover.png"))
	{
		std::cerr << "Sorry could not load back button hover image!\n";
	}

	if (!backButtonClickedTexture.loadFromFile("gfx/buttons/clicked/b_clicked.png"))
	{
		std::cerr << "Sorry could not load back button clicked image!\n";
	}

	backButton.setSize(sf::Vector2f(78.6f, 31.8f));
	backButton.setOrigin(sf::Vector2f(backButton.getLocalBounds().width / 2, backButton.getLocalBounds().height / 2));
	backButton.setPosition(sf::Vector2f(window->getSize().x / 2, window->getSize().y - 25));
	backButton.setTexture(&backButtonTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Load in check box textures, unchecked, checked.
void Options::initCheckBoxTextures()
{
	if (!blankCheckBoxTexture.loadFromFile("gfx/buttons/misc/checkBox.png"))
	{
		std::cerr << "Sorry could not load back button image!\n";
	}

	if (!crossedCheckBoxTexture.loadFromFile("gfx/buttons/misc/xBox.png"))
	{
		std::cerr << "Sorry could not load back button hover image!\n";
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Options::initMuteMusicCheckBox()
{
	muteMusicCheckBox.setSize(sf::Vector2f(25, 26.3));
	muteMusicCheckBox.setOrigin(sf::Vector2f(muteMusicCheckBox.getSize().x / 2, muteMusicCheckBox.getSize().y / 2));
	muteMusicCheckBox.setPosition(sf::Vector2f(500, 219));
	muteMusicCheckBox.setTexture(&blankCheckBoxTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Options::initMuteSFXCheckBox()
{
	muteSFXCheckBox.setSize(sf::Vector2f(25, 26.3));
	muteSFXCheckBox.setOrigin(sf::Vector2f(muteMusicCheckBox.getSize().x / 2, muteMusicCheckBox.getSize().y / 2));
	muteSFXCheckBox.setPosition(sf::Vector2f(500, 286));
	muteSFXCheckBox.setTexture(&blankCheckBoxTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Options::initGodModeCheckBox()
{
	godModeCheckBox.setSize(sf::Vector2f(25, 26.3));
	godModeCheckBox.setOrigin(sf::Vector2f(muteMusicCheckBox.getSize().x / 2, muteMusicCheckBox.getSize().y / 2));
	godModeCheckBox.setPosition(sf::Vector2f(500, 353));
	godModeCheckBox.setTexture(&blankCheckBoxTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Options::checkBackButtonCollisions()
{
	if (checkMouseCollisions(&backButton, mousePos))
	{
		//std::cout << "Hover collision detected with the how to play button!\n";
		backButton.setTexture(&backButtonHoverTexture);

		if (checkMouseCollisions(&backButton, mousePos) && input->isMouseLDown())
		{
			//std::cout << "Clicked on the how to play button!\n";
			backButton.setTexture(&backButtonClickedTexture);
			backButtonClicked = true;
		}
	}
	else
	{
		backButton.setTexture(&backButtonTexture);
	}

	if (backButtonClicked && !input->isMouseLDown())
	{
		audio->playSoundbyName("swoosh");
		backButtonClicked = false;
		setGameState(State::MENU);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Options::checkMuteMusicBoxCollisions()
{
	if (checkMouseCollisions(&muteMusicCheckBox, mousePos) && input->isMouseLDown() && !musicChecked)
	{
		input->setMouseLDown(false);
		//std::cout << "Clicked on the mute music checkbox!\n";
		muteMusicCheckBox.setTexture(&crossedCheckBoxTexture);
		musicChecked = true;
		Level::setMusicMuteAudio(true);			// Mute the game music.
	}

	if (checkMouseCollisions(&muteMusicCheckBox, mousePos) && input->isMouseLDown() && musicChecked)
	{
		input->setMouseLDown(false);
		//std::cout << "Clicked on the mute music checkbox!\n";
		muteMusicCheckBox.setTexture(&blankCheckBoxTexture);
		musicChecked = false;
		Level::setMusicMuteAudio(false);		// Unmute the game music.
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Options::checkMuteSFXBoxCollisions()
{
	if (checkMouseCollisions(&muteSFXCheckBox, mousePos) && input->isMouseLDown() && !sfxChecked)
	{
		input->setMouseLDown(false);
		//std::cout << "Clicked on the mute SFX checkbox!\n";
		muteSFXCheckBox.setTexture(&crossedCheckBoxTexture);
		sfxChecked = true;
		Level::setSFXMuteAudio(true);			// Mute the level sfx.
		Player::setSFXMuteAudio(true);			// Mute the player sfx.
	}

	if (checkMouseCollisions(&muteSFXCheckBox, mousePos) && input->isMouseLDown() && sfxChecked)
	{
		input->setMouseLDown(false);
		//std::cout << "Clicked on the mute SFX checkbox!\n";
		muteSFXCheckBox.setTexture(&blankCheckBoxTexture);
		sfxChecked = false;
		Level::setSFXMuteAudio(false);			// Unmute the level sfx.
		Player::setSFXMuteAudio(false);			// Unmute the player sfx.
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Options::checkGodModeBoxCollisions()
{
	if (checkMouseCollisions(&godModeCheckBox, mousePos) && input->isMouseLDown() && !godModeChecked)
	{
		input->setMouseLDown(false);
		//std::cout << "Clicked on the god mode checkbox!\n";
		godModeCheckBox.setTexture(&crossedCheckBoxTexture);
		godModeChecked = true;
		Player::setGodMode(true);
	}

	if (checkMouseCollisions(&godModeCheckBox, mousePos) && input->isMouseLDown() && godModeChecked)
	{
		input->setMouseLDown(false);
		//std::cout << "Clicked on the god mode checkbox!\n";
		godModeCheckBox.setTexture(&blankCheckBoxTexture);
		godModeChecked = false;
		Player::setGodMode(false);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////