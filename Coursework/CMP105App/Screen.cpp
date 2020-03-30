#include "Screen.h"


Screen::Screen(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud)
{
	window = hwnd;
	input = in;
	gameState = gs;
	audio = aud;

	//setGameState(State::LEVEL);				// THIS WORKS FINE FROM SCREEN.

	// initialise game objects
	//audio->addMusic("sfx/cantina.ogg", "cantina");
}

Screen::~Screen()
{
	std::cout << "Screen object destroyed!\n";
}

void Screen::handleInput(float dt)
{

}

void Screen::update(float dt)
{

}

void Screen::render()
{
	beginDraw();

	endDraw();
}

void Screen::beginDraw()
{
	window->clear(sf::Color(255, 0, 0));
}

void Screen::endDraw()
{
	window->display();
}

State Screen::getGameState()
{
	return gameState->getCurrentState();
}

void Screen::setGameState(State s)
{
	gameState->setCurrentState(s);
}