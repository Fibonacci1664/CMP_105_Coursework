#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/Input.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include <string>
#include <iostream>
#include "Screen.h"

class Credits : public Screen
{
public:
	Credits(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~Credits();

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;

private:
	void initAudio();
	void initCreditsBg();
	void rollCredits(float& dt);

	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();

	sf::RectangleShape creditsBg;
	sf::Texture creditsBgTexture;

	float bottomOfCreditsPos;
};