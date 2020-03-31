#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include "Framework/Input.h"
#include "Framework/GameState.h"
#include "Framework/AudioManager.h"
#include <iostream>
#include <SFML\Graphics\RectangleShape.hpp>

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

	bool checkMouseCollisions(sf::RectangleShape* s1, sf::Vector2f s2);

private:
	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();
};

