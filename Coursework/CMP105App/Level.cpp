// INCLUDES.
#include "Level.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR/S & DESTRUCTOR.
Level::Level(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud) : Screen(hwnd, in, gs, aud)
{
	decr = 192;
	debugMode = true;
	initDebugMode();

	view = sf::View(sf::FloatRect(0.0f, 0.0f, 960, 512));
	window->setView(view);
	scrollSpeed = 300;
	originalViewXCoords = view.getCenter().x;
	xTranslationOfView = 0;

	uiPanel = new UIPanel(hwnd);

	hitPointReductionDelay = 0;
	hitPointsInLevel = 3;
	coinsInLevel = 6;
	keysInLevel = 1;					// If expanding, change this to 3, the vector and collsions are already set up for 3.
	
	mousePos = sf::Vector2f(input->getMouseX(), input->getMouseY());
	fadedOut = false;
	viewMoving = false;
	liftsOn = false;
	paused = false;
	gameOver = false;
	escaped = false;

	initCoins();
	initHitPoints();
	initExitDoor();
	initLifts();
	initParallax();
	initKeys();
	initLever();
	initGroundSpikes();
	initFireTraps();
	initFireLamps();
	//initBackground();
	initAudio();
	initPlayerSpriteTextures();
	initTransFadeRect();
	initPlayer();
}

Level::~Level()
{
	deleteLamps();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS.
void Level::initDebugMode()
{
	debugMode = true;

	initTextBox();

	playerColBox.setFillColor(sf::Color(0, 0, 0, 0));
	playerColBox.setOutlineColor(sf::Color::Red);
	playerColBox.setOutlineThickness(1.0f);
	playerColBox.setPosition(sf::Vector2f((player.getPosition().x + (player.getSize().x / 5)), player.getPosition().y + 18));
	playerColBox.setSize(sf::Vector2f(30, 50));

	OriginBox.setFillColor(sf::Color(0, 0, 0, 0));
	OriginBox.setOutlineColor(sf::Color::Green);
	OriginBox.setOutlineThickness(1.0f);
	OriginBox.setPosition(sf::Vector2f(player.getPosition().x + player.getOrigin().x, player.getPosition().y + player.getOrigin().y));
	OriginBox.setSize(sf::Vector2f(player.getSize().x, player.getSize().y));

	playerPosBox.setFillColor(sf::Color(0, 0, 0, 0));
	playerPosBox.setOutlineColor(sf::Color::White);
	playerPosBox.setOutlineThickness(1.0f);
	playerPosBox.setPosition(sf::Vector2f(player.getPosition().x, player.getPosition().y));
	playerPosBox.setSize(sf::Vector2f(player.getSize().x, player.getSize().y));

	lift_1ColBox.setSize(sf::Vector2f(lift_1.getCollisionBox().width, lift_1.getCollisionBox().height));
	lift_1ColBox.setPosition(sf::Vector2f(lift_1.getPosition().x + 10, lift_1.getPosition().y + 15));
	lift_1ColBox.setFillColor(sf::Color::Transparent);
	lift_1ColBox.setOutlineColor(sf::Color::Magenta);
	lift_1ColBox.setOutlineThickness(1.0f);

	spikeTrapColBox.setFillColor(sf::Color::Transparent);
	spikeTrapColBox.setOutlineColor(sf::Color::Red);
	spikeTrapColBox.setOutlineThickness(1.0f);

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

// handle user input
void Level::handleInput(float dt)
{
	player.handleInput(dt);

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

	if (debugMode)
	{
		window->draw(playerColBox);
		window->draw(OriginBox);
		window->draw(playerPosBox);
		window->draw(lift_1ColBox);
		window->draw(textBox);
		window->draw(text);
		window->draw(spikeTrapColBox);
	}
	
	endDraw();

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

void Level::updateTextOutput()
{
	text.setString("Player X Pos : " + std::to_string(player.getPosition().x) + "Player Y Pos: " + std::to_string(player.getPosition().y) +
		"\n	Mouse X Pos: " + std::to_string(input->getMouseX()) + " Mouse Y Pos: " + std::to_string(input->getMouseY()) +
		"\n	Collision box left Pos: " + std::to_string(player.getCollisionBox().left) + " Collision box top Pos: " + std::to_string(player.getCollisionBox().top));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

void Level::updateKeys(float& dt)
{
	for (int i = 0; i < keys.size(); ++i)
	{
		keys[i]->getKeyAnimation()->animate(dt);
		keys[i]->setTextureRect(keys[i]->getKeyAnimation()->getCurrentFrame());
	}	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::initExitDoor()
{
	if (!exitDoorTexture.loadFromFile("gfx/level/exitDoor.png"))
	{
		std::cerr << "Sorry could not load the exit door image!\n";
	}

	exitDoor.setWindow(window);
	exitDoor.setSize(sf::Vector2f(128, 128));
	exitDoor.setPosition(sf::Vector2f(2703, 345));
	exitDoor.setCollisionBox(32, 32, 64, 64);
	exitDoor.setTexture(&exitDoorTexture);
	exitDoor.setTextureRect(exitDoor.getExitDoorAnimation()->getCurrentFrame());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
		//groundSpikes[i]->setCollisionBox(0, 0, 128, 128);
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
		//fireTraps[i]->setCollisionBox(0, 0, 128, 128);
		fireTraps[i]->setAlive(true);
		fireTraps[i]->setTexture(&fireTrapTexture);
		fireTraps[i]->setTextureRect(fireTraps[i]->getFireTrapAnimation()->getCurrentFrame());

		fireTraps[i]->getFireTrapAnimation()->setPlaying(true);
		fireTraps[i]->getFireTrapAnimation()->setLooping(true);
	}

	fireTraps[0]->setPosition(sf::Vector2f(1080, 164));
	fireTraps[0]->setRotation(90);
	fireTraps[0]->getFireTrapAnimation()->setFrameSpeed(1.0f / 10.0f);
	fireTraps[0]->setSize(sf::Vector2f(64, 64));
	fireTraps[1]->setPosition(sf::Vector2f(2033, 270));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

void Level::updateHitPoints(float& dt)
{
	for (int i = 0; i < 3; ++i)
	{
		hitPoints[i]->getHitPointAnimation()->animate(dt);
		hitPoints[i]->setTextureRect(hitPoints[i]->getHitPointAnimation()->getCurrentFrame());
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

void Level::updateCoins(float& dt)
{
	for (int i = 0; i < coins.size(); ++i)
	{
		coins[i]->getCoinAnimation()->animate(dt);
		coins[i]->setTextureRect(coins[i]->getCoinAnimation()->getCurrentFrame());
	}
}

void Level::updateGroundSpikes(float& dt)
{
	int frameNum = 0;

	// This loop takes care of adjusting the collision box around the spike dependent on how much the spike are showing.
	for (int i = 0; i < groundSpikes.size(); ++i)
	{
		frameNum = groundSpikes[i]->getGroundSpikeAnimation()->getCurrentFrameNumber();

		switch (frameNum)
		{
			case 0:
				groundSpikes[i]->setCollisionBox(0, 0, 0, 0);			// NO COLLISION BOX IF THE SPIKES ARE FULLY RETRACTED.
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

void Level::updateFireTraps(float& dt)
{
	int frameNum = 0;

	// This loop takes care of adjusting the collision box around the spike dependent on how much the spike are showing.
	for (int i = 0; i < fireTraps.size(); ++i)
	{
		frameNum = fireTraps[i]->getFireTrapAnimation()->getCurrentFrameNumber();

		switch (frameNum)
		{
		case 0:
			fireTraps[i]->setCollisionBox(0, 0, 0, 0);			// NO COLLISION BOX IF THERE IS NO FIRE.
			if (debugMode)
			{
				fireTrapColBox.setPosition(sf::Vector2f(fireTraps[i]->getCollisionBox().left, fireTraps[i]->getCollisionBox().top));
				fireTrapColBox.setSize(sf::Vector2f(0, 0));
			}
			break;
		case 1:
			fireTraps[i]->setCollisionBox(60, 80, 10, 15);		// Baby flame.
			if (debugMode)
			{
				fireTrapColBox.setPosition(sf::Vector2f(fireTraps[i]->getCollisionBox().left, fireTraps[i]->getCollisionBox().top));
				fireTrapColBox.setSize(sf::Vector2f(10, 15));
			}
			break;
		case 2:
			fireTraps[i]->setCollisionBox(42, 64, 45, 30);		// Little flame.
			if (debugMode)
			{
				fireTrapColBox.setPosition(sf::Vector2f(fireTraps[i]->getCollisionBox().left, fireTraps[i]->getCollisionBox().top));
				fireTrapColBox.setSize(sf::Vector2f(45, 30));
			}
			break;
		case 3:
			fireTraps[i]->setCollisionBox(32, 42, 64, 54);		// Med flame.
			if (debugMode)
			{
				fireTrapColBox.setPosition(sf::Vector2f(fireTraps[i]->getCollisionBox().left, fireTraps[i]->getCollisionBox().top));
				fireTrapColBox.setSize(sf::Vector2f(64, 54));
			}
			break;
		case 4:
			fireTraps[i]->setCollisionBox(22, 32, 84, 64);		// Large flame.
			if (debugMode)
			{
				fireTrapColBox.setPosition(sf::Vector2f(fireTraps[i]->getCollisionBox().left, fireTraps[i]->getCollisionBox().top));
				fireTrapColBox.setSize(sf::Vector2f(84, 64));
			}
			break;
		case 5:
			fireTraps[i]->setCollisionBox(22, 32, 84, 64);		// Huge flame.
			if (debugMode)
			{
				fireTrapColBox.setPosition(sf::Vector2f(fireTraps[i]->getCollisionBox().left, fireTraps[i]->getCollisionBox().top));
				fireTrapColBox.setSize(sf::Vector2f(84, 64));
			}
			break;
		case 6:
			fireTraps[i]->setCollisionBox(22, 32, 84, 64);		// Bigger flame, but starting to go out.
			if (debugMode)
			{
				fireTrapColBox.setPosition(sf::Vector2f(fireTraps[i]->getCollisionBox().left, fireTraps[i]->getCollisionBox().top));
				fireTrapColBox.setSize(sf::Vector2f(84, 64));
			}
			break;
		case 7:
			fireTraps[i]->setCollisionBox(22, 32, 84, 64);		// Even bigger flame, but going out more.
			if (debugMode)
			{
				fireTrapColBox.setPosition(sf::Vector2f(fireTraps[i]->getCollisionBox().left, fireTraps[i]->getCollisionBox().top));
				fireTrapColBox.setSize(sf::Vector2f(84, 64));
			}
			break;
		case 8:
			fireTraps[i]->setCollisionBox(22, 32, 84, 64);		// Bigger still flame, but almost out.
			if (debugMode)
			{
				fireTrapColBox.setPosition(sf::Vector2f(fireTraps[i]->getCollisionBox().left, fireTraps[i]->getCollisionBox().top));
				fireTrapColBox.setSize(sf::Vector2f(84, 64));
			}
			break;
		case 9:
			fireTraps[i]->setCollisionBox(22, 32, 84, 64);		// Biggest flame, pretty much gone. Thank you for your attention!
			if (debugMode)
			{
				fireTrapColBox.setPosition(sf::Vector2f(fireTraps[i]->getCollisionBox().left, fireTraps[i]->getCollisionBox().top));
				fireTrapColBox.setSize(sf::Vector2f(84, 64));
			}
			break;
		}

		fireTraps[i]->getFireTrapAnimation()->animate(dt);
		fireTraps[i]->setTextureRect(fireTraps[i]->getFireTrapAnimation()->getCurrentFrame());
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

void Level::drawGroundSpikes()
{
	for (int i = 0; i < groundSpikes.size(); ++i)
	{
		window->draw(*groundSpikes[i]);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::drawFireTraps()
{
	for (int i = 0; i < fireTraps.size(); ++i)
	{
		window->draw(*fireTraps[i]);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

void Level::deleteLamps()
{
	// Delete all the allocated lamps of type 3.
	for (int i = 0; i < 18; ++i)
	{
		delete lamps_1[i];
		lamps_1[i] = nullptr;
	}

	// Delete all the allocated lamps of type 3.
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

	// Delete all the allocated lamps of type 3.
	for (int i = 0; i < 9; ++i)
	{
		delete lamps_4[i];
		lamps_4[i] = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::initPlayerSpriteTextures()
{
	if (!player_texture.loadFromFile("gfx/sprites/knight/full_sprite_sheet/knight_sheet_4.png"))
	{
		std::cout << "Error loading player sprite sheet file!\n";
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::initPlayer()
{
	//player.passAndSetCurrentSateFromScreen(gameState);
	player.setInput(input);
	player.setWindow(window);
	player.setSize(sf::Vector2f(58.9f, 68));				// Max size to accomodate ALL sprites.	
	player.setPosition(70, 400);
	player.setTexture(&player_texture);
	player.setCollisionBox(50, 20, 30, 50);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::checkExitDoorCollisions(float& dt)
{
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

		//audio->stopAllMusic();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::checkTileCollisions()
{
	tmm.checkTileCollision(&player);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::checkHitPointCollisions()
{
	if (player.getHitPointsRemaining() < 3)
	{
		// Loop over all the hit points in the level and find the one we collided with.
		for (int i = 0; i < hitPoints.size(); ++i)
		{
			// If the one we've collided with is still alive then collect it and kill it. Killing it prevents further collisions happening.
			if (Collision::checkBoundingBox(hitPoints[i], &player) && hitPoints[i]->isAlive())
			{
				audio->playSoundbyName("hitPoint");
				hitPoints[i]->setAlive(false);
				--hitPointsInLevel;
				player.incrementHitPoints();
				break;
			}
		}
	}	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::checkCoinCollisions()
{
	// Loop over all the coins in the level and find the one we collided with.
	for (int i = 0; i < coins.size(); ++i)
	{
		// If the one we've collided with is still alive then collect it and kill it. Killing it prevents further collisions happening.
		if (Collision::checkBoundingBox(coins[i], &player) && coins[i]->isAlive())
		{
			audio->playSoundbyName("hitPoint");
			coins[i]->setAlive(false);
			--coinsInLevel;
			player.incrementCoinsCollected();
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::checkKeyCollisions()
{
	// Loop over all the keys in the vector and find the one we collided with. Although only one key for the demo, this is built this way to allow for if more keys are drawn.
	for (int i = 0; i < keys.size(); ++i)
	{
		// If the one we've collided with is still alive then collect it and kill it. Killing it prevents further collisions happening.
		if (Collision::checkBoundingBox(keys[i], &player) && keys[i]->isAlive())
		{
			audio->playSoundbyName("hitPoint");
			keys[i]->setAlive(false);
			--keysInLevel;
			player.incrementKeysCollected();
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::checkLeverCollisions()
{
	if (Collision::checkBoundingBox(&lever, &player) && input->isMouseLDown() && lever.isAlive())
	{
		lever.setTexture(leverTextures[1]);
		lever.setAlive(false);
		liftsOn = true;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::checkGroundSpikeCollisions()
{
	// Loop over all the ground spikes in the vector and find the one we collided with.
	for (int i = 0; i < groundSpikes.size(); ++i)
	{
		if (Collision::checkBoundingBox(groundSpikes[i], &player) && hitPointReductionDelay > 1.0f)
		{
			std::cout << "Collided with spikes!\n";

			player.decrementHitPoints();
			player.playSoundByName("umph");
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

void Level::initAudio()
{
	audio->addMusic("sfx/level/castle_music.ogg", "castleMusic");
	audio->addMusic("sfx/level/castle_ambience.ogg", "castleAmbience");
	audio->addSound("sfx/pause/unroll_scroll.ogg", "scroll");
	audio->addSound("sfx/level/death.ogg", "death");
	audio->addSound("sfx/level/collect_hitPoint.ogg", "hitPoint");
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

void Level::checkMusicMuted()
{
	// If the music is not muted then play the level castle music.
	if (!musicMuted)
	{
		if (audio->getMusic()->getStatus() == sf::SoundSource::Stopped)
		{
			audio->playMusicbyName("castleMusic");
			audio->getMusic()->setLoop(true);
		}
	}
	else		// If the music is muted then simple play castle ambience sounds.
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

void Level::initTransFadeRect()
{
	transFade.setSize(sf::Vector2f(view.getSize().x, view.getSize().y));
	transFade.setPosition(sf::Vector2f((view.getCenter().x - view.getSize().x / 2.0f), (view.getCenter().y - view.getSize().y / 2.0f)));
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

void Level::deathCheck()
{
	// If we've died, fade out level screen. NOT ideal having this check carried out in render.
	if (player.getIsDead() && !fadedOut)
	{
		audio->stopAllMusic();
		audio->playSoundbyName("death");
		fadeOutLevel();						// Fade to black.

		if (player.getLives() > 0)
		{
			respawnPlayer();					// Move the player back to starting location while blacked out.		
		}

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

sf::View* Level::getView()
{
	return &view;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::setView(sf::View newView)
{
	view = newView;
}
