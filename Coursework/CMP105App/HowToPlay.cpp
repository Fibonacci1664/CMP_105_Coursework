// INCLUDES.
#include "HowToPlay.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR/S & DESTRUCTOR.
HowToPlay::HowToPlay(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud) : Screen(hwnd, in, gs, aud)
{
	backButtonClicked = false;

	initAudio();
	initHowToPlayBg();
	initBackButton();
}

HowToPlay::~HowToPlay()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS.
void HowToPlay::handleInput(float dt)
{
	checkBackButtonCollisions();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void HowToPlay::update(float dt)
{
	std::cout << "Menu update being called!\n";

	mousePos = sf::Vector2f(input->getMouseX(), input->getMouseY());	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void HowToPlay::render()
{
	beginDraw();
	window->draw(howToPlayBg);
	window->draw(backButton);
	endDraw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void HowToPlay::beginDraw()
{
	//window->clear(sf::Color(0, 255, 0));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void HowToPlay::endDraw()
{
	window->display();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void HowToPlay::initAudio()
{
	audio->addSound("sfx/menu/sword.ogg", "sword");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void HowToPlay::initHowToPlayBg()
{
	if (!howToPlayBgTexture.loadFromFile("gfx/screens/how_to_play_2.png"))
	{
		std::cerr << "Sorry could not how to play background image!\n";
	}

	howToPlayBg.setSize(sf::Vector2f(960, 540));
	howToPlayBg.setTexture(&howToPlayBgTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void HowToPlay::initBackButton()
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

void HowToPlay::checkBackButtonCollisions()
{
	if (checkMouseCollisions(&backButton, mousePos))
	{
		std::cout << "Hover collision detected with the how to play button!\n";
		backButton.setTexture(&backButtonHoverTexture);

		if (checkMouseCollisions(&backButton, mousePos) && input->isMouseLDown())
		{
			std::cout << "Clicked on the how to play button!\n";
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
		audio->playSoundbyName("sword");
		backButtonClicked = false;
		setGameState(State::MENU);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////