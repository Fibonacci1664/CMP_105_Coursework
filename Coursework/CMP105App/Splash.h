#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/Input.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include <string>
#include <iostream>
#include "Screen.h"

class Splash : public Screen
{
public:
	Splash(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~Splash();

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;

private:
	sf::RectangleShape splashBg;
	sf::RectangleShape transFade;
	sf::Texture splashLogoTexture;

	float splashDelay;
	float waitForLogo;
	bool fadedIn;
	bool fadedOut;
	bool switchedStates;

	void initSplashBackground();
	void initTransFadeRect();
	void initAudio();
	void fadeIn();
	void fadeOut();

	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();
};