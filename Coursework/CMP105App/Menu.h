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

	sf::RectangleShape quitButton;
	sf::Texture quitButtonTexture;
	sf::Texture quitButtonHoverTexture;
	sf::Texture quitButtonClickedTexture;

	void initMenuBackground();
	void initHowToPlayButton();
	void initNewGameButton();
	void initQuitButton();
	bool checkMouseCollisions(sf::RectangleShape* s1, sf::Vector2f s2);

	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();
};