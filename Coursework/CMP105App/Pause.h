#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/Input.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include <string>
#include <iostream>
#include "Screen.h"

class Pause : public Screen
{
public:
	Pause(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~Pause();

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;

private:
	sf::RectangleShape pauseTextBox;
	sf::RectangleShape transLayer;
	sf::Text pauseText;
	sf::Font font;

	void initPauseInfo();
	void initTransLayer();

	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();
};