#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/Input.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include <string>
#include <iostream>
#include "Screen.h"

class YouDied : public Screen
{
public:
	YouDied(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~YouDied();

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;

private:
	sf::RectangleShape youDiedRect;
	sf::RectangleShape transFade;
	sf::Texture youDiedTexture;

	bool fadedIn;
	bool fadedOut;
	bool switchedStates;

	void initYouDiedRect();
	void initTransFadeRect();
	void initAudio();
	void fadeIn();
	void fadeOut();

	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();
};