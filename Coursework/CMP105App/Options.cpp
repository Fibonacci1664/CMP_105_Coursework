// INCLUDES.
#include "Options.h"
#include "Level.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR/S & DESTRUCTOR.
Options::Options(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud) : Screen(hwnd, in, gs, aud)
{
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
	std::cout << "Options update being called!\n";

	mousePos = sf::Vector2f(input->getMouseX(), input->getMouseY());
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

void Options::initOptionsBg()
{
	if (!optionsBgTexture.loadFromFile("gfx/screens/options.png"))
	{
		std::cerr << "Sorry could not how to play background image!\n";
	}

	optionsBg.setSize(sf::Vector2f(960, 540));
	optionsBg.setTexture(&optionsBgTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
	muteMusicCheckBox.setPosition(sf::Vector2f(515, 219));
	muteMusicCheckBox.setTexture(&blankCheckBoxTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Options::initMuteSFXCheckBox()
{
	muteSFXCheckBox.setSize(sf::Vector2f(25, 26.3));
	muteSFXCheckBox.setOrigin(sf::Vector2f(muteMusicCheckBox.getSize().x / 2, muteMusicCheckBox.getSize().y / 2));
	muteSFXCheckBox.setPosition(sf::Vector2f(515, 286));
	muteSFXCheckBox.setTexture(&blankCheckBoxTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Options::initGodModeCheckBox()
{
	godModeCheckBox.setSize(sf::Vector2f(25, 26.3));
	godModeCheckBox.setOrigin(sf::Vector2f(muteMusicCheckBox.getSize().x / 2, muteMusicCheckBox.getSize().y / 2));
	godModeCheckBox.setPosition(sf::Vector2f(515, 353));
	godModeCheckBox.setTexture(&blankCheckBoxTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Options::checkBackButtonCollisions()
{
	if (checkMouseCollisions(&backButton, mousePos))
	{
		std::cout << "Hover collision detected with the how to play button!\n";
		backButton.setTexture(&backButtonHoverTexture);

		if (checkMouseCollisions(&backButton, mousePos) && input->isMouseLDown())
		{
			std::cout << "Clicked on the how to play button!\n";
			backButton.setTexture(&backButtonClickedTexture);
			setGameState(State::MENU);
		}
	}
	else
	{
		backButton.setTexture(&backButtonTexture);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Options::checkMuteMusicBoxCollisions()
{
	if (checkMouseCollisions(&muteMusicCheckBox, mousePos) && input->isMouseLDown() && !musicChecked)
	{
		input->setMouseLDown(false);
		std::cout << "Clicked on the mute music checkbox!\n";
		muteMusicCheckBox.setTexture(&crossedCheckBoxTexture);
		musicChecked = true;
		Level::setMusicMuteAudio(true);
	}

	if (checkMouseCollisions(&muteMusicCheckBox, mousePos) && input->isMouseLDown() && musicChecked)
	{
		input->setMouseLDown(false);
		std::cout << "Clicked on the mute music checkbox!\n";
		muteMusicCheckBox.setTexture(&blankCheckBoxTexture);
		musicChecked = false;
		Level::setMusicMuteAudio(false);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Options::checkMuteSFXBoxCollisions()
{
	if (checkMouseCollisions(&muteSFXCheckBox, mousePos) && input->isMouseLDown() && !sfxChecked)
	{
		input->setMouseLDown(false);
		std::cout << "Clicked on the mute SFX checkbox!\n";
		muteSFXCheckBox.setTexture(&crossedCheckBoxTexture);
		sfxChecked = true;
		Player::setSFXMuteAudio(true);
	}

	if (checkMouseCollisions(&muteSFXCheckBox, mousePos) && input->isMouseLDown() && sfxChecked)
	{
		input->setMouseLDown(false);
		std::cout << "Clicked on the mute SFX checkbox!\n";
		muteSFXCheckBox.setTexture(&blankCheckBoxTexture);
		sfxChecked = false;
		Player::setSFXMuteAudio(false);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Options::checkGodModeBoxCollisions()
{
	if (checkMouseCollisions(&godModeCheckBox, mousePos) && input->isMouseLDown() && !godModeChecked)
	{
		input->setMouseLDown(false);
		std::cout << "Clicked on the god mode checkbox!\n";
		godModeCheckBox.setTexture(&crossedCheckBoxTexture);
		godModeChecked = true;
	}

	if (checkMouseCollisions(&godModeCheckBox, mousePos) && input->isMouseLDown() && godModeChecked)
	{
		input->setMouseLDown(false);
		std::cout << "Clicked on the god mode checkbox!\n";
		godModeCheckBox.setTexture(&blankCheckBoxTexture);
		godModeChecked = false;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////