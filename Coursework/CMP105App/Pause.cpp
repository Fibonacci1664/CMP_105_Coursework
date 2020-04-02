// INCLUDES.
#include "Pause.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR/S & DESTRUCTOR.
Pause::Pause(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud) : Screen(hwnd, in, gs, aud)
{
	initAudio();
	initPauseText();
	initScroll();
	initQuitButton();
	initTransLayer();
}

Pause::~Pause()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS.
void Pause::handleInput(float dt)
{
	if (input->isKeyDown(sf::Keyboard::P))
	{
		input->setKeyUp(sf::Keyboard::P);

		audio->playSoundbyName("scroll");

		setGameState(State::LEVEL);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pause::update(float dt)
{
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pause::render()
{
	beginDraw();
	window->draw(transLayer);
	window->draw(pausedTextBox);
	window->draw(scrollBox);
	window->draw(quitButton);
	endDraw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pause::beginDraw()
{
	//window->clear(sf::Color(127, 127, 127, 255));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pause::endDraw()
{
	window->display();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pause::initAudio()
{
	audio->addSound("sfx/pause/unroll_scroll.ogg", "scroll");
}

void Pause::initPauseText()
{
	if (!pausedTexture.loadFromFile("gfx/text/paused_2.png"))
	{
		std::cerr << "Sorry could not load pause text image!\n";
	}

	pausedTextBox.setSize(sf::Vector2f(284.4f, 74.4f));
	pausedTextBox.setOrigin(sf::Vector2f(pausedTextBox.getSize().x / 2.0f, pausedTextBox.getSize().y / 2.0f));
	pausedTextBox.setPosition(sf::Vector2f(window->getSize().x / 2.0f, window->getSize().y / 6.0f));
	pausedTextBox.setTexture(&pausedTexture);
}

void Pause::initScroll()
{
	if (!scrollTexture.loadFromFile("gfx/text/pause_scroll.png"))
	{
		std::cerr << "Sorry could not load scroll image!\n";
	}

	scrollBox.setSize(sf::Vector2f(230.4f, 376.8f));
	scrollBox.setOrigin(sf::Vector2f(scrollBox.getSize().x / 2.0f, scrollBox.getSize().y / 2.0f));
	scrollBox.setPosition(sf::Vector2f(window->getSize().x / 2.0f, window->getSize().y - 195));
	scrollBox.setTexture(&scrollTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pause::initQuitButton()
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
	quitButton.setOrigin(sf::Vector2f(quitButton.getSize().x / 2.0f, quitButton.getSize().y / 2.0f));
	quitButton.setPosition(sf::Vector2f(window->getSize().x / 2.0f, 200));
	quitButton.setTexture(&quitButtonTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pause::initTransLayer()
{
	transLayer.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	transLayer.setFillColor(sf::Color(150, 150, 150, 2.5f));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////