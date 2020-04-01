#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/Input.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include <string>
#include <iostream>
#include "Screen.h"
#include "Player.h"
#include "TileMapManager.h"

static bool musicMuted = false;

class Level : public Screen
{
public:
	Level(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~Level();

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;
	static void setMusicMuteAudio(bool l_muted);

private:
	void checkTileCollisions();
	void initPlayer();
	void initTextures();
	void initTextBox();
	void initAudio();

	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();


	TileMapManager tmm;
	sf::Texture player_texture;
	Player player;
	sf::RectangleShape colBox;
	sf::RectangleShape OriginBox;
	sf::RectangleShape playerPosBox;
	sf::RectangleShape textBox;
	sf::Text text;
	sf::Font font;
};