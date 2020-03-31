// INCLUDES.
#include "Options.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR/S & DESTRUCTOR.
Options::Options(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud) : Screen(hwnd, in, gs, aud)
{
	initOptionsBg();
	initBackButton();
}

Options::~Options()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS.
void Options::handleInput(float dt)
{
	checkBackButtonCollisions();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Options::update(float dt)
{
	std::cout << "Menu update being called!\n";

	mousePos = sf::Vector2f(input->getMouseX(), input->getMouseY());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Options::render()
{
	beginDraw();
	window->draw(optionsBg);
	window->draw(backButton);
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