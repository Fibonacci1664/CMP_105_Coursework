// INCLUDES.
#include "Screen.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR/S & DESTRUCTOR.
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS.
void Screen::handleInput(float dt)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Screen::update(float dt)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Screen::render()
{
	beginDraw();

	endDraw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Screen::beginDraw()
{
	window->clear(sf::Color(255, 0, 0));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Screen::endDraw()
{
	window->display();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

State Screen::getGameState()
{
	return gameState->getCurrentState();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Screen::setGameState(State s)
{
	gameState->setCurrentState(s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Check for bounding box collision with a point/vector2 rather than two boxes.
bool Screen::checkMouseCollisions(sf::RectangleShape* s1, sf::Vector2f s2)
{
	if ((s1->getGlobalBounds().left + s1->getGlobalBounds().width) < s2.x)
	{
		return false;
	}

	if (s1->getGlobalBounds().left > s2.x)
	{
		return false;
	}

	if (s1->getGlobalBounds().top + s1->getGlobalBounds().height < s2.y)
	{
		return false;
	}

	if (s1->getGlobalBounds().top > s2.y)
	{
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////