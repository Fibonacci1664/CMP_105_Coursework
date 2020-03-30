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
	sf::RectangleShape m_pauseTextBox;
	sf::RectangleShape m_transLayer;
	sf::Text m_pauseText;
	sf::Font m_font;

	void initPauseInfo();
	void initTransLayer();

	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();
};