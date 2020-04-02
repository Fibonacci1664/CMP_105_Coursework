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
	sf::Vector2f mousePos;
	sf::RectangleShape transFade;
	bool fadedIn;
	bool howToPlayClicked;
	bool optionsClicked;
	bool newGameClicked;
	bool quitClicked;

	sf::RectangleShape menuBg;
	sf::Texture menuBgTexture;

	sf::RectangleShape howToPlayButton;
	sf::Texture howToPlayButtonTexture;
	sf::Texture howToPlayButtonHoverTexture;
	sf::Texture howToPlayButtonClickedTexture;

	sf::RectangleShape newGameButton;
	sf::Texture newGameButtonTexture;
	sf::Texture newGameButtonHoverTexture;
	sf::Texture newGameButtonClickedTexture;

	sf::RectangleShape optionsButton;
	sf::Texture optinosButtonTexture;
	sf::Texture optionsButtonHoverTexture;
	sf::Texture optionsButtonClickedTexture;

	sf::RectangleShape quitButton;
	sf::Texture quitButtonTexture;
	sf::Texture quitButtonHoverTexture;
	sf::Texture quitButtonClickedTexture;

	void initAudio();
	void initMenuBackground();
	void initHowToPlayButton();
	void initNewGameButton();
	void initOptionsButton();
	void initQuitButton();
	void initTransFadeRect();
	void checkHowToPlayButtonCollisions();
	void checkOptionsButtonCollisions();
	void checkNewGameButtonCollisions();
	void checkQuitButtonCollisions();

	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();
};