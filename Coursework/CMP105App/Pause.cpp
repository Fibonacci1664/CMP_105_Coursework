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
	window->draw(m_transLayer);
	window->draw(m_pauseTextBox);
	window->draw(m_pauseText);
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
	if (!m_font.loadFromFile("font/arial.ttf"))
	{
		std::cerr << "Sorry could not load pause font!\n";
	}

	m_pauseTextBox.setSize(sf::Vector2f(200, 50));
	m_pauseTextBox.setOrigin(m_pauseTextBox.getLocalBounds().width / 2, m_pauseTextBox.getLocalBounds().height / 2);
	m_pauseTextBox.setPosition(sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2));
	m_pauseTextBox.setFillColor(sf::Color(150, 150, 150, 100));
	m_pauseTextBox.setOutlineColor(sf::Color::Black);
	m_pauseTextBox.setOutlineThickness(1.0f);

	m_pauseText.setFont(m_font);
	m_pauseText.setCharacterSize(14);
	m_pauseText.setFillColor(sf::Color::Yellow);
	m_pauseText.setString("PAUSED");
	m_pauseText.setOrigin(m_pauseText.getLocalBounds().width / 2, m_pauseText.getLocalBounds().height);
	m_pauseText.setPosition(sf::Vector2f(m_pauseTextBox.getPosition().x, m_pauseTextBox.getPosition().y));
}

void Pause::initTransLayer()
{
	m_transLayer.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	m_transLayer.setFillColor(sf::Color(150, 150, 150, 2.5f));
}