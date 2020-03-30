#include "Menu.h"


Menu::Menu(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud) : Screen(hwnd, in, gs, aud)
{
	setGameState(State::MENU);

	initMenuBackground();
}

Menu::~Menu()
{

}

void Menu::handleInput(float dt)
{
	if (input->isKeyDown(sf::Keyboard::N))
	{
		input->setKeyUp(sf::Keyboard::N);

		setGameState(State::LEVEL);
	}
}

void Menu::update(float dt)
{

}

void Menu::render()
{
	beginDraw();
	window->draw(m_menuBg);
	endDraw();
}

void Menu::beginDraw()
{
	window->clear(sf::Color(0, 255, 0));
}

void Menu::endDraw()
{
	window->display();
}

void Menu::initMenuBackground()
{
	if (!m_menuBgTexture.loadFromFile("gfx/screens/menu_final.png"))
	{
		std::cerr << "Sorry could not load menu background image!\n";
	}

	m_menuBg.setSize(sf::Vector2f(960, 540));
	m_menuBg.setTexture(&m_menuBgTexture);
}