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
#include "Key.h"

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
	void initLifts();
	void moveLifts(float& dt);
	void initAudio();
	void checkMusicMuted();
	void checkMusicStopped();
	void checkExitDoorCollisions();
	void checkLiftCollisions();
	void initTransFadeRect();
	void initBackground();
	void fadeOutLevel();
	void deathCheck();
	void respawnPlayer();
	void updateView(float& dt);
	void initParallax();
	void updateParallax(float& dt);
	void initFireLamps();
	void initKeys();
	void updateKeys(float& dt);
	void updateLamps(float& dt);
	void drawLamps();
	void deleteLamps();

	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();

	// Lifts.
	GameObject lift_1;
	GameObject lift_2;
	GameObject lift_3;
	GameObject lift_4;
	sf::Texture liftTexture;

	// Keys.
	Key key;
	sf::Texture keyTexture;
	
	// Firelamps.
	std::vector<FireLamp*> lamps_1;
	std::vector<FireLamp*> lamps_2;
	std::vector<FireLamp*> lamps_3;
	std::vector<FireLamp*> lamps_4;
	sf::Texture fireLampTexture;

	// The mouse cursor position.
	sf::Vector2f mousePos;

	sf::RectangleShape exitDoor;
	GameObject exitDoorColBox;
	sf::Texture exitDoorTexture;

	// For fading screens effects.
	sf::RectangleShape transFade;

	// Tilmap.
	TileMapManager tmm;

	// The player.
	Player player;
	sf::Texture player_texture;

	// Debug stuff.
	sf::RectangleShape playerColBox;
	sf::RectangleShape OriginBox;
	sf::RectangleShape playerPosBox;
	sf::RectangleShape lift_1ColBox;
	sf::RectangleShape textBox;
	sf::Text text;
	sf::Font font;

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
	
	// The 'camera' view.
	sf::View view;

	float decr;
	float scrollSpeed;
	bool fadedOut;
	bool fadedIn;
	bool debugMode;
	bool viewMoving;
	bool liftsOn;
	bool paused;
};