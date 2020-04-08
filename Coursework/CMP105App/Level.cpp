// INCLUDES.
#include "Level.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR/S & DESTRUCTOR.
Level::Level(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud) : Screen(hwnd, in, gs, aud)
{
	debugMode = false;
	//initDebugMode();

	view = sf::View(sf::FloatRect(0.0f, 0.0f, 960, 512));
	window->setView(view);
	scrollSpeed = 300;
	mousePos = sf::Vector2f(input->getMouseX(), input->getMouseY());
	fadedOut = false;
	viewMoving = false;

	initParallax();
	initFireLampTexture();
	initFireLamps();
	//initBackground();
	initAudio();
	initPlayerSpriteTextures();
	//initExitDoor();
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

	colBox.setFillColor(sf::Color(0, 0, 0, 0));
	colBox.setOutlineColor(sf::Color::Red);
	colBox.setOutlineThickness(1.0f);
	colBox.setPosition(sf::Vector2f((player.getPosition().x + (player.getSize().x / 5)), player.getPosition().y + 18));
	colBox.setSize(sf::Vector2f(30, 50));

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
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
		setGameState(State::PAUSE);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Update game objects
void Level::update(float dt)
{
	// Keep updating the transparent panel to the current location of the view for when death occurs.
	transFade.setPosition(sf::Vector2f((view.getCenter().x - view.getSize().x / 2.0f), (view.getCenter().y - view.getSize().y / 2.0f)));
	mousePos = sf::Vector2f(input->getMouseX(), input->getMouseY());

	updateLamps(dt);
	checkMusicMuted();
	checkMusicStopped();
	player.update(dt);
	updateView(dt);
	updateParallax(dt);	
	checkTileCollisions();
	checkExitDoorCollisions();
	deathCheck();

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
	window->draw(parallaxDucts);
	window->draw(parallaxColumns);
	tmm.render(window);
	drawLamps();
	window->draw(player);
	window->setView(view);

	if (debugMode)
	{
		window->draw(colBox);
		window->draw(OriginBox);
		window->draw(playerPosBox);
		window->draw(textBox);
		window->draw(text);
	}

	/*window->draw(exitDoor);
	window->draw(exitDoorColBox);*/
	
	endDraw();
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

void Level::updatePlayerBoxes()
{
	if (player.getMovingLeft())
	{
		colBox.setPosition(sf::Vector2f((player.getPosition().x + (player.getSize().x / 3)), player.getPosition().y + 18));
	}
	else
	{
		colBox.setPosition(sf::Vector2f((player.getPosition().x + (player.getSize().x / 5)), player.getPosition().y + 18));
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

	parallaxDucts.setPosition(sf::Vector2f(-1000, 0));
	parallaxDucts.setSize(sf::Vector2f(5760, 512));
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
		parallaxSky.setPosition(sf::Vector2f(parallaxSky.getPosition().x - (30 * dt), parallaxSky.getPosition().y));
		parallaxTrees.setPosition(sf::Vector2f(parallaxTrees.getPosition().x - (50 * dt), parallaxTrees.getPosition().y));
		/*parallaxWindows.setPosition(sf::Vector2f(parallaxWindows.getPosition().x - (20 * dt), parallaxWindows.getPosition().y));
		parallaxDucts.setPosition(sf::Vector2f(parallaxDucts.getPosition().x - (25 * dt), parallaxDucts.getPosition().y));*/
	}

	// We're moving to the left, so scroll the parallax effect to the right.
	if (player.getMovingLeft() && !player.getIdling() && viewMoving)
	{
		parallaxSky.setPosition(sf::Vector2f(parallaxSky.getPosition().x + (30 * dt), parallaxSky.getPosition().y));
		parallaxTrees.setPosition(sf::Vector2f(parallaxTrees.getPosition().x + (50 * dt), parallaxTrees.getPosition().y));
		/*parallaxWindows.setPosition(sf::Vector2f(parallaxWindows.getPosition().x + (20 * dt), parallaxWindows.getPosition().y));
		parallaxDucts.setPosition(sf::Vector2f(parallaxDucts.getPosition().x + (25 * dt), parallaxDucts.getPosition().y));*/
	}

	// If the player stops, stop the effect.
	if (player.getIdling())
	{
		parallaxSky.setPosition(sf::Vector2f(parallaxSky.getPosition().x, parallaxSky.getPosition().y));
		parallaxTrees.setPosition(sf::Vector2f(parallaxTrees.getPosition().x, parallaxTrees.getPosition().y));
		/*parallaxWindows.setPosition(sf::Vector2f(parallaxWindows.getPosition().x, parallaxWindows.getPosition().y));
		parallaxDucts.setPosition(sf::Vector2f(parallaxDucts.getPosition().x, parallaxDucts.getPosition().y));*/
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

void Level::initFireLampTexture()
{
	if (!fireLampTexture.loadFromFile("gfx/level/fire_lamps.png"))
	{
		std::cerr << "Sorry could not load fire lamps image!\n";
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::initFireLamps()
{
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
	player.setPosition(80, 80);
	player.setTexture(&player_texture);
	player.setCollisionBox(50, 20, 30, 50);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::initExitDoor()
{
	if (!exitDoorTexture.loadFromFile("gfx/level/exit_door.png"))
	{
		std::cerr << "Sorry could not load the exit door image!\n";
	}

	exitDoor.setSize(sf::Vector2f(102.4f, 102.4f));
	exitDoor.setOrigin(sf::Vector2f(exitDoor.getSize().x / 2.0f, exitDoor.getSize().y / 2.0f));
	exitDoor.setPosition(sf::Vector2f(400, window->getSize().y - (32 + exitDoor.getSize().y / 2.0f)));
	//exitDoor.setPosition(sf::Vector2f(400, 100));
	exitDoor.setTexture(&exitDoorTexture);

	exitDoorColBox.setSize(sf::Vector2f(25, 25));
	exitDoorColBox.setOrigin(exitDoorColBox.getSize().x / 2.0f, exitDoorColBox.getSize().y / 2.0f);
	exitDoorColBox.setPosition(sf::Vector2f(exitDoor.getPosition().x, exitDoor.getPosition().y + 20));
	exitDoorColBox.setFillColor(sf::Color::Transparent);
	exitDoorColBox.setOutlineColor(sf::Color::Red);
	exitDoorColBox.setOutlineThickness(1.0f);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::checkExitDoorCollisions()
{
	if (player.checkExitDoorCollisions(&exitDoorColBox))
	{
		audio->stopAllMusic();
		setGameState(State::YOU_ESCAPED);
	}
}

void Level::checkTileCollisions()
{
	tmm.checkTileCollision(&player);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::initAudio()
{
	audio->addMusic("sfx/level/castle_music.ogg", "castleMusic");
	audio->addMusic("sfx/level/castle_ambience.ogg", "castleAmbience");
	audio->addSound("sfx/pause/unroll_scroll.ogg", "scroll");
	audio->addSound("sfx/level/death.ogg", "death");
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



// Although named fade in, whats actually occurring is the fading out of an opaque black coloured rectangle shape, the same size as the window, giving the appearance of the level 'fading in'.
/*
 * CANNOT GET THIS TO WORK DUE TO HOW TO HOW LONG THE FADE TAKES WHICH MESSES UP THE
 * COLLISSION CHECKS AND THE PLAYER FALLS THROUGHT THE ENVIRONMENT. NEED A FIXED TIME STEP.
 */
//void Level::fadeInLevel()
//{
//	float decrAlpha = 255;
//
//	//beginDraw();
//
//	while (decrAlpha > 0)
//	{
//		// Controls the speed of fade.
//		decrAlpha -= 0.3f;
//
//		transFade.setFillColor(sf::Color(0, 0, 0, decrAlpha));
//		tmm.render(window);
//		window->draw(player);
//		window->draw(colBox);
//		window->draw(OriginBox);
//		window->draw(playerPosBox);
//		window->draw(textBox);
//		window->draw(text);
//		window->draw(transFade);
//		endDraw();
//	}
//
//	fadedIn = true;
//}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::respawnPlayer()
{
	fadedOut = false;
	player.setRespawned(true);
	player.setIsDead(false);
	player.setPosition(100, 100);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
