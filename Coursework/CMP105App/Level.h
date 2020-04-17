/*
 * This class controls all things related to the Level, including
 *		- Handling level debug information.
 *		- Creating a player.
 *		- Creating the environment.
 *		- Creating the UI.
 *		- Creating the level objects.
 *		- Checking collisions between the player and the level objects.
 *		- Contorlling level music/SFX.
 *		- Checking player death.
 *		- Respawing player.
 *
 * Original @author D. Green.
 *
 * © D. Green. 2020.
 */

 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES.
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
#include "ExitDoor.h"
#include "HitPoint.h"
#include "Coin.h"
#include "UIPanel.h"
#include "GroundSpike.h"
#include "FireTrap.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// STATICS
static bool musicMuted = false;
static bool musicStopped = false;
static bool levelSfxMuted = false;

// The 'camera' view.
static sf::View view;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Level : public Screen
{
public:
	Level(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~Level();

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;

	bool getGameOver();
	void setGameOver(bool l_gameOver);

	// Static functions.
	static sf::View* getView();
	static void setView(sf::View newView);
	static void setMusicMuteAudio(bool l_muted);
	static void setMusicStopped(bool l_stopped);
	static void setSFXMuteAudio(bool l_muted);

private:
	// Init stuff.
	void initDebugMode();
	void initTextBox();
	void initPlayer();
	void initBackground();
	void initLifts();
	void initAudio();
	void initTransFadeRect();
	void initParallax();
	void initFireLamps();
	void initExitDoor();
	void initHitPoints();
	void initCoins();
	void initKeys();
	void initLever();
	void initGroundSpikes();
	void initFireTraps();

	// Check stuff.
	void checkMusicMuted();
	void checkMusicStopped();
	void checkExitDoorCollisions(float& dt);
	void checkLiftCollisions();
	void checkTileCollisions();
	void checkHitPointCollisions();
	void checkCoinCollisions();
	void checkKeyCollisions();
	void checkLeverCollisions();
	void checkGroundSpikeCollisions();
	void checkFireTrapCollisions();

	// Update stuff.
	void updatePlayerBoxes();
	void updateTextOutput();
	void updateView(float& dt);
	void updateParallax(float& dt);
	void updateKeys(float& dt);
	void updateLamps(float& dt);
	void updateHitPoints(float& dt);
	void updateCoins(float& dt);
	void updateGroundSpikes(float& dt);
	void updateFireTraps(float& dt);
	
	// Draw stuff.
	void drawLamps();
	void drawHitPoints();
	void drawCoins();
	void drawKeys();
	void drawGroundSpikes();
	void drawFireTraps();

	// Delete stuff.
	void deleteLamps();
	void deleteHitPoints();
	void deleteCoins();
	void deleteKeys();
	void deleteTextures();
	void deleteGroundSpikes();
	void deleteFireTraps();

	// Misc.
	void moveLifts(float& dt);	
	void fadeOutLevel();
	void deathCheck();
	void respawnPlayer();		

	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// DATA MEMBERS.

	// Lifts.
	GameObject lift_1;
	GameObject lift_2;
	GameObject lift_3;
	GameObject lift_4;
	sf::Texture liftTexture;

	// Ground spike trap.
	std::vector<GroundSpike*> groundSpikes;
	sf::Texture groundSpikeTexture;

	// Fire traps.
	std::vector<FireTrap*> fireTraps;
	sf::Texture fireTrapTexture;

	// Lever for escape.
	GameObject lever;
	std::vector<sf::Texture*> leverTextures;

	// Keys.
	std::vector<Key*> keys;
	sf::Texture keyTexture;

	// Exit door
	ExitDoor exitDoor;
	sf::Texture exitDoorTexture;

	// Hit point
	std::vector<HitPoint*> hitPoints;
	sf::Texture hpTexture;

	// Coins
	std::vector<Coin*> coins;
	sf::Texture coinTexture;
	
	// Firelamps.
	std::vector<FireLamp*> lamps_1;
	std::vector<FireLamp*> lamps_2;
	std::vector<FireLamp*> lamps_3;
	std::vector<FireLamp*> lamps_4;
	sf::Texture fireLampTexture;

	// The mouse cursor position.
	sf::Vector2f mousePos;

	// For fading screens effects.
	sf::RectangleShape transFade;

	// Tilmap.
	TileMapManager tmm;

	// The player.
	Player player;
	sf::Texture player_texture;

	// Debug stuff.
	sf::RectangleShape spikeTrapColBox;
	sf::RectangleShape fireTrapColBox;
	sf::RectangleShape playerColBox;
	sf::RectangleShape OriginBox;
	sf::RectangleShape playerPosBox;
	sf::RectangleShape lift_1ColBox;
	sf::RectangleShape textBox;
	sf::Text text;
	sf::Font font;

	// Stand alone background composite of all 5 images, could be used instead of parallax.
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

	UIPanel* uiPanel;

	float originalViewXCoords;
	float newViewXCoords;
	float xTranslationOfView;
	float scrollSpeed;
	float hitPointReductionDelay;

	int hitPointsInLevel;
	int coinsInLevel;
	int keysInLevel;

	bool fadedOut;
	bool fadedIn;
	bool debugMode;
	bool viewMoving;
	bool liftsOn;
	bool paused;
	bool gameOver;
	bool escaped;
};