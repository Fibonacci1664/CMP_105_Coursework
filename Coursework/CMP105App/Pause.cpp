#include "Pause.h"


Pause::Pause(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud) : Screen(hwnd, in, gs, aud)
{
	initTransLayer();
	initPauseInfo();
}

Pause::~Pause()
{

}

void Pause::handleInput(float dt)
{
	if (input->isKeyDown(sf::Keyboard::P))
	{
		input->setKeyUp(sf::Keyboard::P);

		setGameState(State::LEVEL);
	}
}

void Pause::update(float dt)
{

}

void Pause::render()
{
	beginDraw();
	window->draw(transLayer);
	window->draw(pauseTextBox);
	window->draw(pauseText);
	endDraw();
}

void Pause::beginDraw()
{
	//window->clear(sf::Color(127, 127, 127, 255));
}

void Pause::endDraw()
{
	window->display();
}

void Pause::initPauseInfo()
{
	if (!font.loadFromFile("font/arial.ttf"))
	{
		std::cerr << "Sorry could not load pause font!\n";
	}

	pauseTextBox.setSize(sf::Vector2f(200, 50));
	pauseTextBox.setOrigin(pauseTextBox.getLocalBounds().width / 2, pauseTextBox.getLocalBounds().height / 2);
	pauseTextBox.setPosition(sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2));
	pauseTextBox.setFillColor(sf::Color(150, 150, 150, 100));
	pauseTextBox.setOutlineColor(sf::Color::Black);
	pauseTextBox.setOutlineThickness(1.0f);

	pauseText.setFont(font);
	pauseText.setCharacterSize(14);
	pauseText.setFillColor(sf::Color::Yellow);
	pauseText.setString("PAUSED");
	pauseText.setOrigin(pauseText.getLocalBounds().width / 2, pauseText.getLocalBounds().height);
	pauseText.setPosition(sf::Vector2f(pauseTextBox.getPosition().x, pauseTextBox.getPosition().y));
}

void Pause::initTransLayer()
{
	transLayer.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	transLayer.setFillColor(sf::Color(150, 150, 150, 2.5f));
}