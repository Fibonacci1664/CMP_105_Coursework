#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/Input.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include "Framework/Animation.h"
#include <string>
#include <iostream>
#include "Screen.h"
#include "Player.h"
#include "TileMapManager.h"
#include "FireLamp.h"

static bool musicMuted = false;
static bool musicStopped = false;

class Level : public Screen
{
public:
	Level(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~Level();

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;
	static void setMusicMuteAudio(bool l_muted);
	static void setMusicStopped(bool l_stopped);

private:
	void initDebugMode();
	void updatePlayerBoxes();
	void updateTextOutput();

	void checkTileCollisions();
	void initPlayer();
	void initPlayerSpriteTextures();
	void initExitDoor();
	void initTextBox();
	
	void initAudio();
	void checkMusicMuted();
	void checkMusicStopped();
	void checkExitDoorCollisions();
	void initTransFadeRect();
	void initBackground();
	void fadeOutLevel();
	//void fadeInLevel();
	void deathCheck();
	void respawnPlayer();
	void updateView(float& dt);
	void initParallax();
	void updateParallax(float& dt);
	void initFireLampTexture();
	void initFireLamps();
	void updateLamps(float& dt);
	void drawLamps();
	void deleteLamps();

	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();
	
	std::vector<FireLamp*> lamps_1;
	std::vector<FireLamp*> lamps_2;
	std::vector<FireLamp*> lamps_3;
	std::vector<FireLamp*> lamps_4;

	sf::Vector2f mousePos;

	sf::RectangleShape exitDoor;
	GameObject exitDoorColBox;
	sf::Texture exitDoorTexture;

	sf::RectangleShape transFade;

	TileMapManager tmm;
	sf::Texture fireLampTexture;
	sf::Texture player_texture;
	Player player;
	sf::RectangleShape colBox;
	sf::RectangleShape OriginBox;
	sf::RectangleShape playerPosBox;
	sf::RectangleShape textBox;

	// Stand alone background composite of all 5 images.
	sf::RectangleShape background;
	sf::Texture bgTexture;

	// 5 Images for parallax effect.
	sf::RectangleShape parallaxSky;
	sf::Texture parallaxSkyTexture;
	sf::RectangleShape parallaxTrees;
	sf::Texture parallaxTreesTexture;
	sf::RectangleShape parallaxWindows;
	sf::Texture parallaxWindowsTexture;
	sf::RectangleShape parallaxDucts;
	sf::Texture parallaxDuctsTexture;
	sf::RectangleShape parallaxColumns;
	sf::Texture parallaxColumnsTexture;

	sf::Text text;
	sf::Font font;
	sf::View view;

	float scrollSpeed;
	bool fadedOut;
	bool fadedIn;
	bool debugMode;
	bool viewMoving;
};