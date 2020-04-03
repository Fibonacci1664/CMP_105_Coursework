#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/Input.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include <string>
#include <iostream>
#include "Screen.h"

class YouEscaped : public Screen
{
public:
	YouEscaped(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~YouEscaped();

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;

private:
	sf::RectangleShape youEscapedRect;
	sf::RectangleShape transFade;
	sf::Texture youEscapedTexture;

	bool fadedIn;
	bool fadedOut;
	bool switchedStates;

	void initYouEscapedRect();
	void initTransFadeRect();
	void initAudio();
	void fadeIn();
	void fadeOut();

	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();
};