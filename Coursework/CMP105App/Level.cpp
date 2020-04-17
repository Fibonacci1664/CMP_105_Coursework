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

// INCLUDES.
#include "Level.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR/S & DESTRUCTOR.
Level::Level(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud) : Screen(hwnd, in, gs, aud)
{
	debugMode = false;
	//initDebugMode();

	// View related stuff.
	view = sf::View(sf::FloatRect(0.0f, 0.0f, 960, 512));
	window->setView(view);
	scrollSpeed = 300;
	originalViewXCoords = view.getCenter().x;
	xTranslationOfView = 0;

	// UI Panel along the top of the screen.
	uiPanel = new UIPanel(hwnd);

	// Delay to prevent player collision happening too often.
	hitPointReductionDelay = 0;

	// Track whats left in the level.
	hitPointsInLevel = 3;
	coinsInLevel = 6;
	keysInLevel = 1;					// If expanding, change this to 3, the vector and collsions are already set up for 3.
	
	// For tracking the mouse position, used during debugging.
	mousePos = sf::Vector2f(input->getMouseX(), input->getMouseY());

	fadedOut = false;			// Have we faded out yet.
	viewMoving = false;			// Is the view moving.
	liftsOn = false;			// Are the lifts on.
	paused = false;				// Is the game paused.
	gameOver = false;			// Is it game over yet.
	escaped = false;			// Have we escaped.

	// Initialize everything.
	initPlayer();
	initTextBox();
	initLifts();
	initAudio();
	initTransFadeRect();
	initBackground();
	initParallax();
	initFireLamps();
	initExitDoor();
	initHitPoints();
	initCoins();
	initKeys();
	initLever();
	initGroundSpikes();
	initFireTraps();
}

Level::~Level()
{
	// Delete all the stuff that was created on the heap using 'new'.
	deleteLamps();
	deleteHitPoints();
	deleteCoins();
	deleteKeys();
	deleteTextures();
	deleteGroundSpikes();
	deleteFireTraps();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS.

// START INITIALIZE FUNCTIONS.

// Handle user input
void Level::handleInput(float dt)
{
	player.handleInput(dt);

	// Bring up the pause menu/
	if (input->isKeyDown(sf::Keyboard::P))
	{
		input->setKeyUp(sf::Keyboard::P);
		audio->playSoundbyName("scroll");
		paused = true;
	}

	checkLeverCollisions();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Update game objects
void Level::update(float dt)
{
	hitPointReductionDelay += dt;

	// Keep updating the transparent fade panel to the current location of the view for when death, game over or escape occurs.
	transFade.setPosition(sf::Vector2f((view.getCenter().x - view.getSize().x / 2.0f), (view.getCenter().y - view.getSize().y / 2.0f)));
	mousePos = sf::Vector2f(input->getMouseX(), input->getMouseY());

	newViewXCoords = view.getCenter().x;
	xTranslationOfView = newViewXCoords - originalViewXCoords;

	if (liftsOn)
	{
		moveLifts(dt);
	}

	checkMusicMuted();
	checkMusicStopped();
	player.update(dt);
	updateView(dt);
	updateParallax(dt);
	updateLamps(dt);
	updateKeys(dt);
	updateHitPoints(dt);
	updateCoins(dt);
	updateGroundSpikes(dt);
	updateFireTraps(dt);
	checkTileCollisions();
	checkLiftCollisions();
	checkExitDoorCollisions(dt);
	checkHitPointCollisions();
	checkCoinCollisions();
	checkKeyCollisions();
	checkGroundSpikeCollisions();
	checkFireTrapCollisions();
	deathCheck();
	uiPanel->update(dt, player.getHitPointsRemaining(), player.getLives(), player.getCoinsCollected(), player.getKeysCollected(), xTranslationOfView);

	if (debugMode)
	{
		updatePlayerBoxes();
		updateTextOutput();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Render level
void Level::render()
{
	beginDraw();

	//window->draw(background);
	window->draw(parallaxSky);
	window->draw(parallaxTrees);
	window->draw(parallaxWindows);
	window->draw(parallaxColumns);
	drawLamps();
	tmm.render(window);
	window->draw(parallaxDucts);
	window->draw(lift_1);
	window->draw(lift_2);
	window->draw(lift_3);
	window->draw(lift_4);
	drawHitPoints();
	drawCoins();
	drawKeys();
	drawGroundSpikes();
	drawFireTraps();
	window->draw(exitDoor);
	window->draw(lever);
	uiPanel->render();

	window->draw(player);
	window->setView(view);

	// Draw all the debug stuff.
	if (debugMode)
	{
		window->draw(playerColBox);
		window->draw(OriginBox);
		window->draw(playerPosBox);
		window->draw(lift_1ColBox);
		window->draw(textBox);
		window->draw(text);
		window->draw(spikeTrapColBox);
		window->draw(fireTrapColBox);
	}

	endDraw();

	// Both of these if statements are here, so there are no draw calls after switching state.
	if (paused)
	{
		//fadeOutLevel();
		//view.reset(sf::FloatRect(0.0f, 0.0f, 960, 512));
		setGameState(State::PAUSE);
		paused = false;
	}

	if (escaped)
	{
		fadeOutLevel();						// Fade to black.
		view.reset(sf::FloatRect(0.0f, 0.0f, 960, 512));
		setGameState(State::YOU_ESCAPED);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Begins rendering to the back buffer. Background colour set to light black.
void Level::beginDraw()
{
	window->clear(sf::Color::Blue);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Ends rendering to the back buffer, and swaps buffer to the screen.
void Level::endDraw()
{
	window->display();
}

// For debugging.
void Level::initDebugMode()
{
	debugMode = true;

	// A text box with information about player position and mouse position etc.
	initTextBox();

	// A red box around the players collision area.
	playerColBox.setFillColor(sf::Color(0, 0, 0, 0));
	playerColBox.setOutlineColor(sf::Color::Red);
	playerColBox.setOutlineThickness(1.0f);
	playerColBox.setPosition(sf::Vector2f((player.getPosition().x + (player.getSize().x / 5)), player.getPosition().y + 18));
	playerColBox.setSize(sf::Vector2f(30, 50));

	// A green box around the player, starting from the players origin.
	OriginBox.setFillColor(sf::Color(0, 0, 0, 0));
	OriginBox.setOutlineColor(sf::Color::Green);
	OriginBox.setOutlineThickness(1.0f);
	OriginBox.setPosition(sf::Vector2f(player.getPosition().x + player.getOrigin().x, player.getPosition().y + player.getOrigin().y));
	OriginBox.setSize(sf::Vector2f(player.getSize().x, player.getSize().y));

	// A white box around the player.
	playerPosBox.setFillColor(sf::Color(0, 0, 0, 0));
	playerPosBox.setOutlineColor(sf::Color::White);
	playerPosBox.setOutlineThickness(1.0f);
	playerPosBox.setPosition(sf::Vector2f(player.getPosition().x, player.getPosition().y));
	playerPosBox.setSize(sf::Vector2f(player.getSize().x, player.getSize().y));
	
	// A magenta box around the one of the lifts.
	lift_1ColBox.setSize(sf::Vector2f(lift_1.getCollisionBox().width, lift_1.getCollisionBox().height));
	lift_1ColBox.setPosition(sf::Vector2f(lift_1.getPosition().x + 10, lift_1.getPosition().y + 15));
	lift_1ColBox.setFillColor(sf::Color::Transparent);
	lift_1ColBox.setOutlineColor(sf::Color::Magenta);
	lift_1ColBox.setOutlineThickness(1.0f);

	// A red box around the ground spike trap.
	spikeTrapColBox.setFillColor(sf::Color::Transparent);
	spikeTrapColBox.setOutlineColor(sf::Color::Red);
	spikeTrapColBox.setOutlineThickness(1.0f);

	// A red box around the fire trap.
	fireTrapColBox.setFillColor(sf::Color::Transparent);
	fireTrapColBox.setOutlineColor(sf::Color::Red);
	fireTrapColBox.setOutlineThickness(1.0f);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// For debugging.
void Level::initTextBox()
{
	if (!font.loadFromFile("font/arial.ttf"))
	{
		std::cout << "Error loading font\n";
	}

	textBox.setSize(sf::Vector2f(600, 100));
	textBox.setOrigin(textBox.getSize().x / 2, textBox.getSize().y / 2);
	textBox.setFillColor(sf::Color(0, 0, 0, 255));
	textBox.setPosition(sf::Vector2f(window->getSize().x / 2, 75));

	// A text box containing info about player position and mouse position.
	text.setFont(font);
	text.setString("Player X Pos : " + std::to_string(player.getPosition().x) + "Player Y Pos: " + std::to_string(player.getPosition().y) +
		"\n	Mouse X Pos: " + std::to_string(input->getMouseX()) + " Mouse Y Pos: " + std::to_string(input->getMouseY()) +
		"\n	Collision box left Pos: " + std::to_string(player.getCollisionBox().left) + " Collision box top Pos: " + std::to_string(player.getCollisionBox().top));
	text.setCharacterSize(14);
	text.setOrigin(sf::Vector2f(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2));
	text.setPosition(sf::Vector2f(textBox.getPosition().x, textBox.getPosition().y - 30));
	text.setFillColor(sf::Color::Red);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Create the player
void Level::initPlayer()
{
	if (!player_texture.loadFromFile("gfx/sprites/knight/full_sprite_sheet/knight_sheet_4.png"))
	{
		std::cout << "Error loading player sprite sheet file!\n";
	}

	//player.passAndSetCurrentSateFromScreen(gameState);
	player.setInput(input);
	player.setWindow(window);
	player.setSize(sf::Vector2f(58.9f, 68));				// Max size to accomodate ALL sprites.	
	player.setPosition(70, 400);
	player.setTexture(&player_texture);
	player.setCollisionBox(50, 20, 30, 50);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Init background, this will end up in its own class, because i will layer and do paralax ultimately but need it here for now to test the view.
void Level::initBackground()
{
	if (!bgTexture.loadFromFile("gfx/screens/castle_bg.png"))
	{
		std::cerr << "Sorry could not load bg image for level!\n";
	}

	background.setSize(sf::Vector2f(2880, 512));
	background.setTexture(&bgTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Create all the lifts in the level.
void Level::initLifts()
{
	if (!liftTexture.loadFromFile("gfx/level/lift.png"))
	{
		std::cerr << "Sorry could not load lift image!\n";
	}
	
	lift_1.setSize(sf::Vector2f(64, 64));
	lift_1.setVelocity(sf::Vector2f(0, 50));
	lift_1.setTexture(&liftTexture);
	lift_1.setCollisionBox(10, 15, 44, 34);
	lift_1.setPosition(sf::Vector2f(200, 179));
	
	lift_2.setSize(sf::Vector2f(64, 64));
	lift_2.setVelocity(sf::Vector2f(0, 50));
	lift_2.setTexture(&liftTexture);
	lift_2.setCollisionBox(10, 15, 44, 34);
	lift_2.setPosition(sf::Vector2f(1150, 240));

	lift_3.setSize(sf::Vector2f(64, 64));
	lift_3.setVelocity(sf::Vector2f(0, 50));
	lift_3.setTexture(&liftTexture);
	lift_3.setCollisionBox(10, 15, 44, 34);
	lift_3.setPosition(sf::Vector2f(1300, 290));

	lift_4.setSize(sf::Vector2f(64, 64));
	lift_4.setVelocity(sf::Vector2f(0, 50));
	lift_4.setTexture(&liftTexture);
	lift_4.setCollisionBox(10, 15, 44, 34);
	lift_4.setPosition(sf::Vector2f(2450, 270));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Load in all audio. Music is streamed from path.
void Level::initAudio()
{
	audio->addMusic("sfx/level/castle_music.ogg", "castleMusic");
	audio->addMusic("sfx/level/castle_ambience.ogg", "castleAmbience");
	audio->addSound("sfx/level/death.ogg", "death");
	audio->addSound("sfx/level/collect_hitPoint.ogg", "hitPoint");
	audio->addSound("sfx/level/collect_coin.ogg", "coin");
	audio->addSound("sfx/level/collect_key.ogg", "key");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Create a transparent panel used for fading effects.
void Level::initTransFadeRect()
{
	transFade.setSize(sf::Vector2f(view.getSize().x, view.getSize().y));
	transFade.setPosition(sf::Vector2f((view.getCenter().x - view.getSize().x / 2.0f), (view.getCenter().y - view.getSize().y / 2.0f)));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Load in images for parallax. NOT ALL ARE USED THOUGH AS THE MID GROUND (PLAYER AREA) IS STATIONARY.
void Level::initParallax()
{
	// Parallax is layered in this order. Far to near.

	// Sky image.
	if (!parallaxSkyTexture.loadFromFile("gfx/level/parallax/sky_5760.png"))
	{
		std::cerr << "Sorry could not load bg sky image!\n";
	}

	parallaxSky.setPosition(sf::Vector2f(-1000, 0));
	parallaxSky.setSize(sf::Vector2f(5760, 512));
	parallaxSky.setTexture(&parallaxSkyTexture);

	// Trees image
	if (!parallaxTreesTexture.loadFromFile("gfx/level/parallax/trees_5760.png"))
	{
		std::cerr << "Sorry could not load bg trees image!\n";
	}

	parallaxTrees.setPosition(sf::Vector2f(-1000, 0));
	parallaxTrees.setSize(sf::Vector2f(5760, 512));
	parallaxTrees.setTexture(&parallaxTreesTexture);

	// Castle walls image.
	if (!parallaxWindowsTexture.loadFromFile("gfx/level/parallax/walls_5760.png"))
	{
		std::cerr << "Sorry could not load bg wall image!\n";
	}

	parallaxWindows.setPosition(sf::Vector2f(-1000, 0));
	parallaxWindows.setSize(sf::Vector2f(5760, 512));
	parallaxWindows.setTexture(&parallaxWindowsTexture);

	// Ducts image.
	if (!parallaxDuctsTexture.loadFromFile("gfx/level/parallax/ducts_5760.png"))
	{
		std::cerr << "Sorry could not load bg ducts image!\n";
	}

	parallaxDucts.setPosition(sf::Vector2f(-1000, 265));
	parallaxDucts.setSize(sf::Vector2f(5760, 256));
	parallaxDucts.setTexture(&parallaxDuctsTexture);

	// Columns image.
	if (!parallaxColumnsTexture.loadFromFile("gfx/level/2880_x_514/columns_2880.png"))
	{
		std::cerr << "Sorry could not load bg columns image!\n";
	}

	// The last image is purposfully set smaller as the view is checked against this so the view stops scrolling when it reaches the end of this image.
	parallaxColumns.setPosition(sf::Vector2f(-50, 0));
	parallaxColumns.setSize(sf::Vector2f(2880, 512));
	parallaxColumns.setTexture(&parallaxColumnsTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Create all the fire lamps in the environment background.
void Level::initFireLamps()
{
	if (!fireLampTexture.loadFromFile("gfx/level/fire_lamps.png"))
	{
		std::cerr << "Sorry could not load fire lamps image!\n";
	}

	// Create 10 fire lamps of type lamp_1, set them to play on loop.
	for (int i = 0; i < 18; ++i)
	{
		FireLamp* lamp_1 = new FireLamp;
		lamps_1.push_back(lamp_1);

		lamps_1[i]->setWindow(window);
		lamps_1[i]->setSize(sf::Vector2f(64, 64));
		lamps_1[i]->setPosition(sf::Vector2f(45 + (i * 158) + i, 280));
		lamps_1[i]->setTexture(&fireLampTexture);
		lamps_1[i]->setTextureRect(lamps_1[i]->getLamp_1()->getCurrentFrame());
		lamps_1[i]->getLamp_1()->setPlaying(true);
		lamps_1[i]->getLamp_1()->setLooping(true);
	}

	// Create 10 fire lamps of type lamp_1, set them to play on loop.
	for (int i = 0; i < 9; ++i)
	{
		FireLamp* lamp_2 = new FireLamp;
		lamps_2.push_back(lamp_2);

		lamps_2[i]->setWindow(window);
		lamps_2[i]->setSize(sf::Vector2f(64, 64));
		lamps_2[i]->setPosition(sf::Vector2f(132 + (i * 319) + i, 80));
		lamps_2[i]->setTexture(&fireLampTexture);
		lamps_2[i]->setTextureRect(lamps_2[i]->getLamp_2()->getCurrentFrame());
		lamps_2[i]->getLamp_2()->setPlaying(true);
		lamps_2[i]->getLamp_2()->setLooping(true);
	}

	// Create 10 fire lamps of type lamp_1, set them to play on loop.
	for (int i = 0; i < 9; ++i)
	{
		FireLamp* lamp_3 = new FireLamp;
		lamps_3.push_back(lamp_3);

		lamps_3[i]->setWindow(window);
		lamps_3[i]->setSize(sf::Vector2f(64, 64));
		lamps_3[i]->setPosition(sf::Vector2f(79 + (i * 319) + i, 40));
		lamps_3[i]->setTexture(&fireLampTexture);
		lamps_3[i]->setTextureRect(lamps_3[i]->getLamp_3()->getCurrentFrame());
		lamps_3[i]->getLamp_3()->setPlaying(true);
		lamps_3[i]->getLamp_3()->setLooping(true);
	}

	// Create 10 fire lamps of type lamp_1, set them to play on loop.
	for (int i = 0; i < 9; ++i)
	{
		FireLamp* lamp_4 = new FireLamp;
		lamps_4.push_back(lamp_4);

		lamps_4[i]->setWindow(window);
		lamps_4[i]->setSize(sf::Vector2f(64, 64));
		lamps_4[i]->setPosition(sf::Vector2f(232 + (i * 320) + i, 5));
		lamps_4[i]->setTexture(&fireLampTexture);
		lamps_4[i]->setTextureRect(lamps_4[i]->getLamp_4()->getCurrentFrame());
		lamps_4[i]->getLamp_4()->setPlaying(true);
		lamps_4[i]->getLamp_4()->setLooping(true);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Create the exit door.
void Level::initExitDoor()
{
	if (!exitDoorTexture.loadFromFile("gfx/level/exitDoor.png"))
	{
		std::cerr << "Sorry could not load the exit door image!\n";
	}

	exitDoor.setWindow(window);
	exitDoor.setSize(sf::Vector2f(128, 128));
	exitDoor.setPosition(sf::Vector2f(2703, 345));
	exitDoor.setCollisionBox(32, 102, 64, 28);
	exitDoor.setTexture(&exitDoorTexture);
	exitDoor.setTextureRect(exitDoor.getExitDoorAnimation()->getCurrentFrame());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Create the hit points.
void Level::initHitPoints()
{
	if (!hpTexture.loadFromFile("gfx/level/hitPoint.png"))
	{
		std::cerr << "Sorry could not load hit point image!\n";
	}

	// Create 3 hit points.
	for (int i = 0; i < 3; ++i)
	{
		HitPoint* hp = new HitPoint;
		hitPoints.push_back(hp);

		hitPoints[i]->setWindow(window);
		hitPoints[i]->setSize(sf::Vector2f(32, 32));
		hitPoints[i]->setCollisionBox(0, 0, 32, 32);
		hitPoints[i]->setAlive(true);
		hitPoints[i]->setTexture(&hpTexture);
		hitPoints[i]->setTextureRect(hitPoints[i]->getHitPointAnimation()->getCurrentFrame());
		hitPoints[i]->getHitPointAnimation()->setPlaying(true);
		hitPoints[i]->getHitPointAnimation()->setLooping(true);
	}

	hitPoints[0]->setPosition(sf::Vector2f(82, 80));
	hitPoints[1]->setPosition(sf::Vector2f(970, 80));
	hitPoints[2]->setPosition(sf::Vector2f(2127, 320));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Create the coins.
void Level::initCoins()
{
	if (!coinTexture.loadFromFile("gfx/level/coin.png"))
	{
		std::cerr << "Sorry could not load coin image!\n";
	}

	// Create 6 coins.
	for (int i = 0; i < 6; ++i)
	{
		Coin* coin = new Coin;
		coins.push_back(coin);

		coins[i]->setWindow(window);
		coins[i]->setSize(sf::Vector2f(32, 32));
		coins[i]->setCollisionBox(0, 0, 32, 32);
		coins[i]->setAlive(true);
		coins[i]->setTexture(&coinTexture);
		coins[i]->setTextureRect(coins[i]->getCoinAnimation()->getCurrentFrame());
		coins[i]->getCoinAnimation()->setPlaying(true);
		coins[i]->getCoinAnimation()->setLooping(true);
	}

	coins[0]->setPosition(sf::Vector2f(525, 128));
	coins[1]->setPosition(sf::Vector2f(910, 80));
	coins[2]->setPosition(sf::Vector2f(1030, 80));
	coins[3]->setPosition(sf::Vector2f(1523, 192));
	coins[4]->setPosition(sf::Vector2f(2163, 64));
	coins[5]->setPosition(sf::Vector2f(2575, 320));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Create all the key/s in the level.
void Level::initKeys()
{
	if (!keyTexture.loadFromFile("gfx/level/key.png"))
	{
		std::cerr << "Sorry could not load key image!\n";
	}

	// Create 3 keys, although for this demo, only 1 key will be given a position, drawn, and need to be collected.
	for (int i = 0; i < 3; ++i)
	{
		Key* key = new Key;
		keys.push_back(key);

		keys[i]->setWindow(window);
		keys[i]->setSize(sf::Vector2f(64, 64));
		keys[i]->setCollisionBox(0, 0, 64, 64);
		keys[i]->setAlive(true);
		keys[i]->setTexture(&keyTexture);
		keys[i]->setTextureRect(keys[i]->getKeyAnimation()->getCurrentFrame());
		keys[i]->getKeyAnimation()->setPlaying(true);
		keys[i]->getKeyAnimation()->setLooping(true);
	}

	keys[0]->setPosition(sf::Vector2f(64, 128));			// Only giving the first key a position as we're only having 1 key in this demo level.
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Create the lever.
void Level::initLever()
{
	// Load in the two lever textures.
	for (int i = 0; i < 2; ++i)
	{
		sf::Texture* leverTexture = new sf::Texture;
		leverTextures.push_back(leverTexture);

		if (!leverTextures[i]->loadFromFile("gfx/level/" + std::to_string(i) + "_lever.png"))
		{
			std::cerr << "Sorry could not load lever image number " << i << '\n';
		}
	}

	lever.setSize(sf::Vector2f(64, 48));
	lever.setPosition(sf::Vector2f(2352, 400));
	lever.setCollisionBox(0, 0, 64, 64);
	lever.setAlive(true);
	lever.setTexture(leverTextures[0]);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Create the ground spike trap.
void Level::initGroundSpikes()
{
	if (!groundSpikeTexture.loadFromFile("gfx/level/traps/ground_spike.png"))
	{
		std::cerr << "Sorry could not load ground spike image!\n";
	}

	// Create 2 ground spike traps.
	for (int i = 0; i < 2; ++i)
	{
		GroundSpike* groundSpike = new GroundSpike;
		groundSpikes.push_back(groundSpike);

		groundSpikes[i]->setWindow(window);
		groundSpikes[i]->setSize(sf::Vector2f(128, 128));
		groundSpikes[i]->setAlive(true);
		groundSpikes[i]->setTexture(&groundSpikeTexture);
		groundSpikes[i]->setTextureRect(groundSpikes[i]->getGroundSpikeAnimation()->getCurrentFrame());
		groundSpikes[i]->getGroundSpikeAnimation()->setPlaying(true);
		groundSpikes[i]->getGroundSpikeAnimation()->setLooping(false);
	}

	groundSpikes[0]->setPosition(sf::Vector2f(1024, 352));
	groundSpikes[1]->setPosition(sf::Vector2f(1830, 160));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Create the fire traps.
void Level::initFireTraps()
{
	if (!fireTrapTexture.loadFromFile("gfx/level/traps/fire_trap.png"))
	{
		std::cerr << "Sorry could not load fire trap image!\n";
	}

	// Create 2 fire traps with default values.
	for (int i = 0; i < 2; ++i)
	{
		FireTrap* fireTrap = new FireTrap;
		fireTraps.push_back(fireTrap);

		fireTraps[i]->setWindow(window);
		fireTraps[i]->setSize(sf::Vector2f(128, 128));
		fireTraps[i]->setAlive(true);
		fireTraps[i]->setTexture(&fireTrapTexture);
		fireTraps[i]->setTextureRect(fireTraps[i]->getFireTrapAnimation()->getCurrentFrame());

		fireTraps[i]->getFireTrapAnimation()->setPlaying(true);
		fireTraps[i]->getFireTrapAnimation()->setLooping(true);
	}

	fireTraps[0]->setPosition(sf::Vector2f(753, 228));
	fireTraps[0]->getFireTrapAnimation()->setFrameSpeed(1.0f / 10.0f);
	fireTraps[1]->setPosition(sf::Vector2f(2033, 270));
}

// END INITIALIZE FUNCTIONS.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// START CHECKING FUNCTIONS.

// Check if the player chose to mute music in the options.
void Level::checkMusicMuted()
{
	// If the music is not muted then play the level castle music.
	if (!musicMuted)
	{
		if (audio->getMusic()->getStatus() == sf::SoundSource::Stopped)
		{
			audio->playMusicbyName("castleMusic");
			audio->getMusic()->setVolume(25);
			audio->getMusic()->setLoop(true);
		}
	}
	else		// If the music is muted then simply play castle ambience sounds by default.
	{
		if (audio->getMusic()->getStatus() == sf::SoundSource::Stopped)
		{
			audio->playMusicbyName("castleAmbience");
			audio->getMusic()->setLoop(true);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::checkMusicStopped()
{
	if (musicStopped)
	{
		audio->stopAllMusic();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Check is the player has collided with the exit door.
void Level::checkExitDoorCollisions(float& dt)
{
	// Check if the player has collided with the door AND there are no keys in the level.
	if (Collision::checkBoundingBox(&exitDoor, &player) && keysInLevel == 0)
	{
		exitDoor.getExitDoorAnimation()->setPlaying(true);
		exitDoor.getExitDoorAnimation()->animate(dt);
		exitDoor.setTextureRect(exitDoor.getExitDoorAnimation()->getCurrentFrame());

		// If we get to frame 5 we've played the whole animation, so exit.
		if (exitDoor.getExitDoorAnimation()->getCurrentFrameNumber() == 4)
		{
			gameOver = true;
			escaped = true;
		}

		audio->stopAllMusic();
	}
	else if (Collision::checkBoundingBox(&exitDoor, &player) && keysInLevel == 1)	// If we collide with the door and there are still keys in the level, then death.
	{
		player.decrementLives();
		player.setIsDead(true);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::checkLiftCollisions()
{
	if (Collision::checkBoundingBox(&player, &lift_1))
	{
		player.collisionResponse(&lift_1);
	}

	if (Collision::checkBoundingBox(&player, &lift_2))
	{
		player.collisionResponse(&lift_2);
	}

	if (Collision::checkBoundingBox(&player, &lift_3))
	{
		player.collisionResponse(&lift_3);
	}

	if (Collision::checkBoundingBox(&player, &lift_4))
	{
		player.collisionResponse(&lift_4);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Check all tile collisions.
void Level::checkTileCollisions()
{
	tmm.checkTileCollision(&player);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Check if the player has collided with any hitpoint icons.
void Level::checkHitPointCollisions()
{
	// Only check is the player is able to collect a hit point, i.e. their hit points are < 3.
	if (player.getHitPointsRemaining() < 3)
	{
		// Loop over all the hit points in the level and find the one we collided with.
		for (int i = 0; i < hitPoints.size(); ++i)
		{
			// If the one we've collided with is still alive then collect it and kill it. Killing it prevents further collisions happening.
			if (Collision::checkBoundingBox(hitPoints[i], &player) && hitPoints[i]->isAlive())
			{
				if (!levelSfxMuted)
				{
					audio->playSoundbyName("hitPoint");
				}

				hitPoints[i]->setAlive(false);
				--hitPointsInLevel;
				player.incrementHitPoints();
				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Check if the player has collided with any coins.
void Level::checkCoinCollisions()
{
	// Loop over all the coins in the level and find the one we collided with.
	for (int i = 0; i < coins.size(); ++i)
	{
		// If the one we've collided with is still alive then collect it and kill it. Killing it prevents further collisions happening.
		if (Collision::checkBoundingBox(coins[i], &player) && coins[i]->isAlive())
		{
			if (!levelSfxMuted)
			{
				audio->playSoundbyName("coin");
			}

			coins[i]->setAlive(false);
			--coinsInLevel;
			player.incrementCoinsCollected();
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Check is the player has collided with any key/s.
void Level::checkKeyCollisions()
{
	// Loop over all the keys in the vector and find the one we collided with. Although only one key for the demo, this is built this way to allow for if more keys are drawn.
	for (int i = 0; i < keys.size(); ++i)
	{
		// If the one we've collided with is still alive then collect it and kill it. Killing it prevents further collisions happening.
		if (Collision::checkBoundingBox(keys[i], &player) && keys[i]->isAlive())
		{
			if (!levelSfxMuted)
			{
				audio->playSoundbyName("key");
			}

			keys[i]->setAlive(false);
			--keysInLevel;
			player.incrementKeysCollected();
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Check is the player has collided with the level.
void Level::checkLeverCollisions()
{
	// If the player has collided AND they have 'attacked' AND the lever has not been hit before, i.e. it's still alive.
	if (Collision::checkBoundingBox(&lever, &player) && input->isMouseLDown() && lever.isAlive())
	{
		lever.setTexture(leverTextures[1]);
		lever.setAlive(false);
		liftsOn = true;		// Now switch the lifts on.
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Check if the player has collided with any of the ground spike traps.
void Level::checkGroundSpikeCollisions()
{
	// Loop over all the ground spikes in the vector and find the one we collided with.
	for (int i = 0; i < groundSpikes.size(); ++i)
	{
		// If we've collided and the the last time we collided is > 1 second ago.
		if (Collision::checkBoundingBox(groundSpikes[i], &player) && hitPointReductionDelay > 1.0f)
		{
			//std::cout << "Collided with spikes!\n";

			player.decrementHitPoints();

			if (!levelSfxMuted)
			{
				player.playSoundByName("umph");
			}

			hitPointReductionDelay = 0;

			// If moving right, bounce left.
			if (player.getMovingRight())
			{
				/*
				 * Check if the last spike collision caused our hit points to reach zero, this prevents the stepvelocity
				 * being applied on the collision that causes death and prevent the player respawning with the applied step velocity.
				 */
				if (player.getHitPointsRemaining() == 0)
				{
					player.decrementLives();
					player.setIsDead(true);
				}
				else
				{
					player.injuryBounce();
				}
			}
			else if (player.getMovingLeft())		// If moving left, bounce right.
			{
				if (player.getHitPointsRemaining() == 0)
				{
					player.decrementLives();
					player.setIsDead(true);
				}
				else
				{
					player.injuryBounce();
				}
			}

			// If we've found the spike we collided with, we dont need to check the others, so stop looping.
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Check if the player has collided with any of the fire traps.
void Level::checkFireTrapCollisions()
{
	// Loop over all the ground spikes in the vector and find the one we collided with.
	for (int i = 0; i < fireTraps.size(); ++i)
	{
		if (Collision::checkBoundingBox(fireTraps[i], &player) && hitPointReductionDelay > 1.0f)
		{
			//std::cout << "Collided with fire trap!\n";

			player.decrementHitPoints();

			if (!levelSfxMuted)
			{
				player.playSoundByName("umph");
			}

			hitPointReductionDelay = 0;

			// If moving right, bounce left.
			if (player.getMovingRight())
			{
				/*
				 * Check if the last fire trap collision caused our hit points to reach zero, this prevents the stepvelocity
				 * being applied on the collision that causes death and prevent the player respawning with the applied step velocity.
				 */
				if (player.getHitPointsRemaining() == 0)
				{
					player.decrementLives();
					player.setIsDead(true);
				}
				else
				{
					player.injuryBounce();
				}
			}
			else if (player.getMovingLeft())		// If moving left, bounce right.
			{
				if (player.getHitPointsRemaining() == 0)
				{
					player.decrementLives();
					player.setIsDead(true);
				}
				else
				{
					player.injuryBounce();
				}
			}

			// If we've found the fire trap we collided with, we dont need to check the others, so stop looping.
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// END CHECKING FUNCTIONS.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// START UPDATING FUNCTIONS.

/*
 * Update the players collision box based on direction facing, req. because the frame has an amount of transparent pixels around the character,
 * required for when animations get larger, this however causes a change in exact coords when the frame is flipped and so the collision box needs update
 * by the amount the the actual image of the player is offset by once flipped, it's no tmuch but it's enough.
 */
void Level::updatePlayerBoxes()
{
	if (player.getMovingLeft())
	{
		playerColBox.setPosition(sf::Vector2f((player.getPosition().x + (player.getSize().x / 3)), player.getPosition().y + 18));
	}
	else
	{
		playerColBox.setPosition(sf::Vector2f((player.getPosition().x + (player.getSize().x / 5)), player.getPosition().y + 18));
	}

	playerPosBox.setPosition(sf::Vector2f(player.getPosition().x, player.getPosition().y));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Keep udating the text output during debugging.
void Level::updateTextOutput()
{
	text.setString("Player X Pos : " + std::to_string(player.getPosition().x) + "Player Y Pos: " + std::to_string(player.getPosition().y) +
		"\n	Mouse X Pos: " + std::to_string(input->getMouseX()) + " Mouse Y Pos: " + std::to_string(input->getMouseY()) +
		"\n	Collision box left Pos: " + std::to_string(player.getCollisionBox().left) + " Collision box top Pos: " + std::to_string(player.getCollisionBox().top));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Move the view based on player position.
void Level::updateView(float& dt)
{
	// If the player gets past half way accross the screen, start scrolling.
	if (player.getPosition().x > view.getCenter().x)
	{
		view.move(scrollSpeed * dt, 0);
		viewMoving = true;

		// If the view gets to the end of the bg image the stop scrolling.
		if (view.getCenter().x > (parallaxColumns.getSize().x - view.getSize().x / 2))
		{
			view.move(-scrollSpeed * dt, 0);
			viewMoving = false;
		}
	}
	else if (player.getPosition().x < (view.getCenter().x - view.getSize().x / 5.0f))	// If once we've gone passed the centre and then move backwards again and our x becomes less than the views centre we then scroll the view backwards.
	{
		// Scroll the view backwards.
		view.move(-scrollSpeed * dt, 0);
		viewMoving = true;

		// If the view gets to zero in the x then stop scrolling.
		if (view.getCenter().x < (0 + view.getSize().x / 2))
		{
			view.move(scrollSpeed * dt, 0);
			viewMoving = false;
		}
	}
	else
	{
		viewMoving = false;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Move the images that are used during the parallax, first attempt at parallax, not sure i got right.
void Level::updateParallax(float& dt)
{
	// We're moving to the right, so scroll the parallax effect to the left.
	if (player.getMovingRight() && !player.getIdling() && viewMoving)
	{
		parallaxSky.setPosition(sf::Vector2f(parallaxSky.getPosition().x - (10 * dt), parallaxSky.getPosition().y));
		parallaxTrees.setPosition(sf::Vector2f(parallaxTrees.getPosition().x - (20 * dt), parallaxTrees.getPosition().y));
		//parallaxWindows.setPosition(sf::Vector2f(parallaxWindows.getPosition().x - (20 * dt), parallaxWindows.getPosition().y));
		parallaxDucts.setPosition(sf::Vector2f(parallaxDucts.getPosition().x - (30 * dt), parallaxDucts.getPosition().y));
	}

	// We're moving to the left, so scroll the parallax effect to the right.
	if (player.getMovingLeft() && !player.getIdling() && viewMoving)
	{
		parallaxSky.setPosition(sf::Vector2f(parallaxSky.getPosition().x + (10 * dt), parallaxSky.getPosition().y));
		parallaxTrees.setPosition(sf::Vector2f(parallaxTrees.getPosition().x + (20 * dt), parallaxTrees.getPosition().y));
		//parallaxWindows.setPosition(sf::Vector2f(parallaxWindows.getPosition().x + (20 * dt), parallaxWindows.getPosition().y));
		parallaxDucts.setPosition(sf::Vector2f(parallaxDucts.getPosition().x + (30 * dt), parallaxDucts.getPosition().y));
	}

	// If the player stops, stop the effect.
	if (player.getIdling())
	{
		parallaxSky.setPosition(sf::Vector2f(parallaxSky.getPosition().x, parallaxSky.getPosition().y));
		parallaxTrees.setPosition(sf::Vector2f(parallaxTrees.getPosition().x, parallaxTrees.getPosition().y));
		//parallaxWindows.setPosition(sf::Vector2f(parallaxWindows.getPosition().x, parallaxWindows.getPosition().y));
		parallaxDucts.setPosition(sf::Vector2f(parallaxDucts.getPosition().x, parallaxDucts.getPosition().y));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Animate the key/s.
void Level::updateKeys(float& dt)
{
	for (int i = 0; i < keys.size(); ++i)
	{
		keys[i]->getKeyAnimation()->animate(dt);
		keys[i]->setTextureRect(keys[i]->getKeyAnimation()->getCurrentFrame());
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Animate all the envirornment lamps.
void Level::updateLamps(float& dt)
{
	for (int i = 0; i < 18; ++i)
	{
		lamps_1[i]->getLamp_1()->animate(dt);
		lamps_1[i]->setTextureRect(lamps_1[i]->getLamp_1()->getCurrentFrame());
	}

	for (int i = 0; i < 9; ++i)
	{
		lamps_2[i]->getLamp_2()->animate(dt);
		lamps_2[i]->setTextureRect(lamps_2[i]->getLamp_2()->getCurrentFrame());
	}

	for (int i = 0; i < 9; ++i)
	{
		lamps_3[i]->getLamp_3()->animate(dt);
		lamps_3[i]->setTextureRect(lamps_3[i]->getLamp_3()->getCurrentFrame());
	}

	for (int i = 0; i < 9; ++i)
	{
		lamps_4[i]->getLamp_4()->animate(dt);
		lamps_4[i]->setTextureRect(lamps_4[i]->getLamp_4()->getCurrentFrame());
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Animate the hit point icons.
void Level::updateHitPoints(float& dt)
{
	for (int i = 0; i < 3; ++i)
	{
		hitPoints[i]->getHitPointAnimation()->animate(dt);
		hitPoints[i]->setTextureRect(hitPoints[i]->getHitPointAnimation()->getCurrentFrame());
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Animate the coins.
void Level::updateCoins(float& dt)
{
	for (int i = 0; i < coins.size(); ++i)
	{
		coins[i]->getCoinAnimation()->animate(dt);
		coins[i]->setTextureRect(coins[i]->getCoinAnimation()->getCurrentFrame());
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * Animate the ground spikes, the collision boxes are also created and update here to corrospond with which frame the animation is on, this way the
 * collision box changes with the animation.
 */
void Level::updateGroundSpikes(float& dt)
{
	int frameNum = 0;

	// This loop takes care of adjusting the collision box around the spike dependent on how much the spike are showing.
	for (int i = 0; i < groundSpikes.size(); ++i)
	{
		frameNum = groundSpikes[i]->getGroundSpikeAnimation()->getCurrentFrameNumber();

		switch (frameNum)
		{
		case 0: // A COLLISION BOX OF 0, 0, WHICH BELIEVE IT OR NOT STILL REGISTERS A HIT, SO I COULDN'T USE (0,0,0,0) BUT INSTEAD PUT IT IN THE MIDDLE OF THE SPRITE AND 'INSIDE' THE GROUND SO IT'S IMPOSSIBLE TO HIT.
			groundSpikes[i]->setCollisionBox(64, -10, 0, 0);
			if (debugMode)
			{
				spikeTrapColBox.setPosition(sf::Vector2f(groundSpikes[i]->getCollisionBox().left, groundSpikes[i]->getCollisionBox().top));
				spikeTrapColBox.setSize(sf::Vector2f(0, 0));
			}
			break;
		case 1:
			groundSpikes[i]->setCollisionBox(60, 80, 10, 15);		// One solitary spike in the centre of the trap, only just protruding.
			if (debugMode)
			{
				spikeTrapColBox.setPosition(sf::Vector2f(groundSpikes[i]->getCollisionBox().left, groundSpikes[i]->getCollisionBox().top));
				spikeTrapColBox.setSize(sf::Vector2f(10, 15));
			}
			break;
		case 2:
			groundSpikes[i]->setCollisionBox(42, 64, 45, 30);		// All but the outer most spike now show but not all the way out.
			if (debugMode)
			{
				spikeTrapColBox.setPosition(sf::Vector2f(groundSpikes[i]->getCollisionBox().left, groundSpikes[i]->getCollisionBox().top));
				spikeTrapColBox.setSize(sf::Vector2f(45, 30));
			}
			break;
		case 3:
			groundSpikes[i]->setCollisionBox(32, 42, 64, 54);		// All spike now showing but not all the way out.
			if (debugMode)
			{
				spikeTrapColBox.setPosition(sf::Vector2f(groundSpikes[i]->getCollisionBox().left, groundSpikes[i]->getCollisionBox().top));
				spikeTrapColBox.setSize(sf::Vector2f(64, 54));
			}
			break;
		case 4:
			groundSpikes[i]->setCollisionBox(22, 32, 84, 64);		// All spikes showing and all the way out.
			if (debugMode)
			{
				spikeTrapColBox.setPosition(sf::Vector2f(groundSpikes[i]->getCollisionBox().left, groundSpikes[i]->getCollisionBox().top));
				spikeTrapColBox.setSize(sf::Vector2f(84, 64));
			}
			break;
		}

		groundSpikes[i]->getGroundSpikeAnimation()->animate(dt);
		groundSpikes[i]->setTextureRect(groundSpikes[i]->getGroundSpikeAnimation()->getCurrentFrame());
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * Animate the fire trap, the collision boxes are also created and update here to corrospond with which frame the animation is on, this way the
 * collision box changes with the animation.
 */
void Level::updateFireTraps(float& dt)
{
	int frameNum = 0;

	// This loop takes care of adjusting the collision box around the spike dependent on how much the spike are showing.
	for (int i = 0; i < fireTraps.size(); ++i)
	{
		frameNum = fireTraps[i]->getFireTrapAnimation()->getCurrentFrameNumber();

		switch (frameNum)
		{
		case 0: // A COLLISION BOX OF 0, 0, WHICH BELIEVE IT OR NOT STILL REGISTERS A HIT, SO I COULDN'T USE (0,0,0,0) BUT INSTEAD PUT IT IN THE MIDDLE OF THE SPRITE AND 'INSIDE' THE WALL IT'S ATTACHED TO SO IT'S IMPOSSIBLE TO HIT.
			fireTraps[i]->setCollisionBox(-10, 64, 0, 0);
			if (debugMode)
			{
				fireTrapColBox.setPosition(sf::Vector2f(fireTraps[i]->getCollisionBox().left, fireTraps[i]->getCollisionBox().top));
				fireTrapColBox.setSize(sf::Vector2f(0, 0));
			}
			break;
		case 1:
			fireTraps[i]->setCollisionBox(0, 50, 30, 30);		// Baby flame.
			if (debugMode)
			{
				fireTrapColBox.setPosition(sf::Vector2f(fireTraps[i]->getCollisionBox().left, fireTraps[i]->getCollisionBox().top));
				fireTrapColBox.setSize(sf::Vector2f(30, 30));
			}
			break;
		case 2:
			fireTraps[i]->setCollisionBox(0, 50, 40, 30);		// Little flame.
			if (debugMode)
			{
				fireTrapColBox.setPosition(sf::Vector2f(fireTraps[i]->getCollisionBox().left, fireTraps[i]->getCollisionBox().top));
				fireTrapColBox.setSize(sf::Vector2f(40, 30));
			}
			break;
		case 3:
			fireTraps[i]->setCollisionBox(0, 50, 60, 30);		// Med flame.
			if (debugMode)
			{
				fireTrapColBox.setPosition(sf::Vector2f(fireTraps[i]->getCollisionBox().left, fireTraps[i]->getCollisionBox().top));
				fireTrapColBox.setSize(sf::Vector2f(60, 30));
			}
			break;
		case 4:
			fireTraps[i]->setCollisionBox(0, 50, 80, 30);		// Large flame.
			if (debugMode)
			{
				fireTrapColBox.setPosition(sf::Vector2f(fireTraps[i]->getCollisionBox().left, fireTraps[i]->getCollisionBox().top));
				fireTrapColBox.setSize(sf::Vector2f(80, 30));
			}
			break;
		case 5:
			fireTraps[i]->setCollisionBox(0, 50, 100, 30);		// Huge flame.
			if (debugMode)
			{
				fireTrapColBox.setPosition(sf::Vector2f(fireTraps[i]->getCollisionBox().left, fireTraps[i]->getCollisionBox().top));
				fireTrapColBox.setSize(sf::Vector2f(100, 30));
			}
			break;
		case 6:
			fireTraps[i]->setCollisionBox(0, 50, 110, 30);		// Bigger flame, but starting to go out.
			if (debugMode)
			{
				fireTrapColBox.setPosition(sf::Vector2f(fireTraps[i]->getCollisionBox().left, fireTraps[i]->getCollisionBox().top));
				fireTrapColBox.setSize(sf::Vector2f(110, 30));
			}
			break;
		case 7:
			fireTraps[i]->setCollisionBox(0, 50, 120, 30);		// Even bigger flame, but going out more.
			if (debugMode)
			{
				fireTrapColBox.setPosition(sf::Vector2f(fireTraps[i]->getCollisionBox().left, fireTraps[i]->getCollisionBox().top));
				fireTrapColBox.setSize(sf::Vector2f(120, 30));
			}
			break;
		case 8:
			fireTraps[i]->setCollisionBox(0, 50, 125, 30);		// Bigger still flame, but almost out.
			if (debugMode)
			{
				fireTrapColBox.setPosition(sf::Vector2f(fireTraps[i]->getCollisionBox().left, fireTraps[i]->getCollisionBox().top));
				fireTrapColBox.setSize(sf::Vector2f(125, 30));
			}
			break;
		case 9:
			fireTraps[i]->setCollisionBox(0, 50, 128, 30);		// Biggest flame, pretty much gone. Thank you for your attention!
			if (debugMode)
			{
				fireTrapColBox.setPosition(sf::Vector2f(fireTraps[i]->getCollisionBox().left, fireTraps[i]->getCollisionBox().top));
				fireTrapColBox.setSize(sf::Vector2f(128, 30));
			}
			break;
		}

		fireTraps[i]->getFireTrapAnimation()->animate(dt);
		fireTraps[i]->setTextureRect(fireTraps[i]->getFireTrapAnimation()->getCurrentFrame());
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// END UPDATING FUNCTIONS.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// START DRAW FUNCTIONS.

// We're only drawing 2 types of lamp here as any more was just over kill. Left in though for future use, bigger levels etc.
void Level::drawLamps()
{
	/*for (int i = 0; i < 18; ++i)
	{
		window->draw(*lamps_1[i]);
	}*/

	/*for (int i = 0; i < 9; ++i)
	{
		window->draw(*lamps_2[i]);
	}*/

	for (int i = 0; i < 9; ++i)
	{
		window->draw(*lamps_3[i]);
	}

	for (int i = 0; i < 9; ++i)
	{
		window->draw(*lamps_4[i]);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Draw all the hit points in the level.
void Level::drawHitPoints()
{
	for (int i = 0; i < hitPoints.size(); ++i)
	{
		if (hitPoints[i]->isAlive())
		{
			window->draw(*hitPoints[i]);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Draw all the coins in the level.
void Level::drawCoins()
{
	for (int i = 0; i < coins.size(); ++i)
	{
		if (coins[i]->isAlive())
		{
			window->draw(*coins[i]);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Draw all the key/s in the level.
void Level::drawKeys()
{
	for (int i = 0; i < keysInLevel; ++i)
	{
		if (keys[i]->isAlive())
		{
			window->draw(*keys[i]);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Draw all the ground spike traps.
void Level::drawGroundSpikes()
{
	for (int i = 0; i < groundSpikes.size(); ++i)
	{
		window->draw(*groundSpikes[i]);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Draw all the fire traps.
void Level::drawFireTraps()
{
	for (int i = 0; i < fireTraps.size(); ++i)
	{
		window->draw(*fireTraps[i]);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// END DRAW FUNCTIONS.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// START DELETE FUNCTIONS.

// Delete all the lamps.
void Level::deleteLamps()
{
	// Delete all the allocated lamps of type 1.
	for (int i = 0; i < 18; ++i)
	{
		delete lamps_1[i];
		lamps_1[i] = nullptr;
	}

	// Delete all the allocated lamps of type 2.
	for (int i = 0; i < 9; ++i)
	{
		delete lamps_2[i];
		lamps_2[i] = nullptr;
	}

	// Delete all the allocated lamps of type 3.
	for (int i = 0; i < 9; ++i)
	{
		delete lamps_3[i];
		lamps_3[i] = nullptr;
	}

	// Delete all the allocated lamps of type 4.
	for (int i = 0; i < 9; ++i)
	{
		delete lamps_4[i];
		lamps_4[i] = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Delete all the hit points.
void Level::deleteHitPoints()
{
	// Delete all the hit points.
	for (int i = 0; i < hitPoints.size(); ++i)
	{
		delete hitPoints[i];
		hitPoints[i] = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Delete all the coins.
void Level::deleteCoins()
{
	// Delete all the hit points.
	for (int i = 0; i < hitPoints.size(); ++i)
	{
		delete hitPoints[i];
		hitPoints[i] = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Delete all the key/s.
void Level::deleteKeys()
{
	// Delete all the keys.
	for (int i = 0; i < keys.size(); ++i)
	{
		delete keys[i];
		keys[i] = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Delete all the textures that were used as numbers for tracking the amount of coins collected.
void Level::deleteTextures()
{
	// Delete all the lever textures.
	for (int i = 0; i < leverTextures.size(); ++i)
	{
		delete leverTextures[i];
		leverTextures[i] = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Delete all the ground spike traps.
void Level::deleteGroundSpikes()
{
	// Delete all the ground spikes.
	for (int i = 0; i < groundSpikes.size(); ++i)
	{
		delete groundSpikes[i];
		groundSpikes[i] = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Delete all the fire traps.
void Level::deleteFireTraps()
{
	// Delete all the fire traps.
	for (int i = 0; i < fireTraps.size(); ++i)
	{
		delete fireTraps[i];
		fireTraps[i] = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// END DELETE FUNCTIONS.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// START MISC. FUNCTIONS.

// Move all the lifts in the level.
void Level::moveLifts(float& dt)
{
	lift_1.setPosition(sf::Vector2f(lift_1.getPosition().x, (lift_1.getPosition().y + (lift_1.getVelocity().y * dt))));
	lift_2.setPosition(sf::Vector2f(lift_2.getPosition().x, (lift_2.getPosition().y + (lift_2.getVelocity().y * dt))));
	lift_3.setPosition(sf::Vector2f(lift_3.getPosition().x, (lift_3.getPosition().y + (lift_3.getVelocity().y * dt))));
	lift_4.setPosition(sf::Vector2f(lift_4.getPosition().x, (lift_4.getPosition().y + (lift_4.getVelocity().y * dt))));
	//lift_1ColBox.setPosition(sf::Vector2f(lift_1.getPosition().x + 10, lift_1.getPosition().y + 15));

	if (lift_1.getPosition().y > 300)
	{
		lift_1.setVelocity(sf::Vector2f(0, -50));
	}

	if (lift_1.getPosition().y < 180)
	{
		lift_1.setVelocity(sf::Vector2f(0, 50));
	}

	if (lift_2.getPosition().y > 300)
	{
		lift_2.setVelocity(sf::Vector2f(0, -50));
	}

	if (lift_2.getPosition().y < 180)
	{
		lift_2.setVelocity(sf::Vector2f(0, 50));
	}

	if (lift_3.getPosition().y > 300)
	{
		lift_3.setVelocity(sf::Vector2f(0, -50));
	}

	if (lift_3.getPosition().y < 180)
	{
		lift_3.setVelocity(sf::Vector2f(0, 50));
	}

	if (lift_4.getPosition().y > 384)
	{
		lift_4.setVelocity(sf::Vector2f(0, -50));
	}

	if (lift_4.getPosition().y < 120)
	{
		lift_4.setVelocity(sf::Vector2f(0, 50));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Although named fade out, whats actually occurring is the fading in of an opaque black coloured rectangle shape, the same size as the window, giving the appearance of the level 'fading out'.
void Level::fadeOutLevel()
{
	float incrAlpha = 0;

	while (incrAlpha < 255)
	{
		// Controls the speed of fade.
		incrAlpha += 0.01f;

		transFade.setFillColor(sf::Color(0, 0, 0, incrAlpha));
		window->draw(transFade);
		endDraw();

		if (incrAlpha > 20)
		{
			//fadedOut = true;
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Check if something in the level caused our player to be dead.
void Level::deathCheck()
{
	if (player.getIsDead() && !fadedOut)
	{
		audio->stopAllMusic();

		if (!levelSfxMuted)
		{
			audio->playSoundbyName("death");
		}

		fadeOutLevel();						// Fade to black.

		// If we still have lives, repsawn.
		if (player.getLives() > 0)
		{
			respawnPlayer();					// Move the player back to starting location while blacked out.		
		}

		// Reset the view so we can see the you died screen.
		view.reset(sf::FloatRect(0.0f, 0.0f, 960, 512));
		setGameState(State::YOU_DIED);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::respawnPlayer()
{
	fadedOut = false;
	player.respawn();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// END MISC. FUNCTIONS.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// START STATIC FUNCTIONS.

sf::View* Level::getView()
{
	return &view;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::setView(sf::View newView)
{
	view = newView;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::setMusicMuteAudio(bool l_muted)
{
	musicMuted = l_muted;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::setMusicStopped(bool l_stopped)
{
	musicStopped = l_stopped;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::setSFXMuteAudio(bool l_muted)
{
	levelSfxMuted = l_muted;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// END STATIC FUNCTIONS.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// START GETTERS / SETTERS.

bool Level::getGameOver()
{
	return gameOver;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::setGameOver(bool l_gameOver)
{
	gameOver = l_gameOver;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////