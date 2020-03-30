#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/Input.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include <string>
#include <iostream>
#include "Screen.h"

class Menu : public Screen
{
public:
	Menu(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~Menu();

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;

private:
	sf::RectangleShape m_menuBg;
	sf::Texture m_menuBgTexture;

	void initMenuBackground();

	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();
};