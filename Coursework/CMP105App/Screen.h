/*
 * This is the parent class of all other 'screens'.
 *
 * Original @author D. Green.
 *
 * © D. Green. 2020.
 */

 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES.
#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include "Framework/Input.h"
#include "Framework/GameState.h"
#include "Framework/AudioManager.h"
#include "Framework/GameObject.h"
#include <iostream>
#include <SFML\Graphics\RectangleShape.hpp>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Screen
{
public:
	Screen(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~Screen();

	virtual void handleInput(float dt);
	virtual void update(float dt);
	virtual void render();

	virtual State getGameState();
	virtual void setGameState(State s);	

protected:
	// Default variables for level class.
	sf::RenderWindow* window;
	Input* input;
	GameState* gameState;
	AudioManager* audio;

	// Custom cursor.
	GameObject customCursor;
	sf::Texture cursorTexture;

	bool checkMouseCollisions(sf::RectangleShape* s1, sf::Vector2f s2);

private:
	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();

	void initCursor();
};

