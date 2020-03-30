#include "Menu.h"


Menu::Menu(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud) : Screen(hwnd, in, gs, aud)
{
	initMenuBackground();
	initHowToPlayButton();
	initNewGameButton();
	initQuitButton();
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
	window->draw(menuBg);
	window->draw(howToPlayButton);
	window->draw(newGameButton);
	window->draw(quitButton);
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
	if (!menuBgTexture.loadFromFile("gfx/screens/menu_final.png"))
	{
		std::cerr << "Sorry could not load menu background image!\n";
	}

	menuBg.setSize(sf::Vector2f(960, 540));
	menuBg.setTexture(&menuBgTexture);
}

void Menu::initHowToPlayButton()
{
	if (!howToPlayButtonTexture.loadFromFile("gfx/buttons/how_to_play_button.png"))
	{
		std::cerr << "Sorry could not load how to play button image!\n";
	}

	howToPlayButton.setSize(sf::Vector2f(176.4f, 31.8f));
	howToPlayButton.setPosition(sf::Vector2f(75, 170));
	howToPlayButton.setTexture(&howToPlayButtonTexture);
}

void Menu::initNewGameButton()
{
	if (!newGameButtonTexture.loadFromFile("gfx/buttons/new_game_button.png"))
	{
		std::cerr << "Sorry could not load new game button image!\n";
	}

	newGameButton.setSize(sf::Vector2f(138, 31.8f));
	newGameButton.setPosition(sf::Vector2f(95, 210));
	newGameButton.setTexture(&newGameButtonTexture);
}

void Menu::initQuitButton()
{
	if (!quitButtonTexture.loadFromFile("gfx/buttons/quit_button.png"))
	{
		std::cerr << "Sorry could not load quit button image!\n";
	}

	quitButton.setSize(sf::Vector2f(75.6f, 31.8f));
	quitButton.setPosition(sf::Vector2f(125, 250));
	quitButton.setTexture(&quitButtonTexture);
}