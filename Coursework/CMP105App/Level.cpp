// INCLUDES.
#include "Level.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR/S & DESTRUCTOR.
Level::Level(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud) : Screen(hwnd, in, gs, aud)
{
	fadedOut = false;
	fadedIn = false;

	initAudio();
	initTextures();
	initTextBox();
	initTransFadeRect();

	// initialise game objects
	initPlayer();

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

Level::~Level()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS.
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

// handle user input
void Level::handleInput(float dt)
{
	player.handleInput(dt);

	if (input->isKeyDown(sf::Keyboard::P))
	{
		input->setKeyUp(sf::Keyboard::P);

		setGameState(State::PAUSE);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Update game objects
void Level::update(float dt)
{
	checkMusicMuted();
	checkMusicStopped();

	//if (player.getIsDead() && fadedOut)
	//{
	//	respawnPlayer();
	//}

	player.update(dt);
	checkTileCollisions();

	if (player.getMovingLeft())
	{
		colBox.setPosition(sf::Vector2f((player.getPosition().x + (player.getSize().x / 3)), player.getPosition().y + 18));
	}
	else
	{
		colBox.setPosition(sf::Vector2f((player.getPosition().x + (player.getSize().x / 5)), player.getPosition().y + 18));
	}

	playerPosBox.setPosition(sf::Vector2f(player.getPosition().x, player.getPosition().y));

	text.setString("Player X Pos : " + std::to_string(player.getPosition().x) + "Player Y Pos: " + std::to_string(player.getPosition().y) +
		"\n	Mouse X Pos: " + std::to_string(input->getMouseX()) + " Mouse Y Pos: " + std::to_string(input->getMouseY()) +
		"\n	Collision box left Pos: " + std::to_string(player.getCollisionBox().left) + " Collision box top Pos: " + std::to_string(player.getCollisionBox().top));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Render level
void Level::render()
{
	//// If not faded in level screen, then DO IT!
	//if (!fadedIn)
	//{
	//	fadeInLevel();
	//}

	beginDraw();
	tmm.render(window);
	window->draw(player);
	window->draw(colBox);
	//window->draw(OriginBox);
	window->draw(playerPosBox);
	window->draw(textBox);
	window->draw(text);
	endDraw();

	//// After fading in just draw normally.
	//if (!player.getIsDead() && fadedIn)
	//{
	//	beginDraw();
	//	tmm.render(window);
	//	window->draw(player);
	//	window->draw(colBox);
	//	//window->draw(OriginBox);
	//	window->draw(playerPosBox);
	//	window->draw(textBox);
	//	window->draw(text);
	//	endDraw();
	//}

	//// If we've died, fade out level screen.
	//if (player.getIsDead() && !fadedOut)
	//{
	//	audio->stopAllMusic();
	//	fadeOutLevel();
	//	setGameState(State::YOU_DIED);
	//}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Begins rendering to the back buffer. Background colour set to light black.
void Level::beginDraw()
{
	window->clear(sf::Color::Black);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Ends rendering to the back buffer, and swaps buffer to the screen.
void Level::endDraw()
{
	window->display();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::initTextures()
{
	if (!player_texture.loadFromFile("gfx/sprites/knight/full_sprite_sheet/knight_sheet_4.png"))
	{
		std::cout << "Error loading player sprite sheet file!\n";
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::initPlayer()
{
	player.passAndSetCurrentSateFromScreen(gameState);
	player.setInput(input);
	player.setWindow(window);
	player.setSize(sf::Vector2f(58.9f, 68));				// Max size to accomodate ALL sprites.	
	player.setPosition(100, 100);
	player.setTexture(&player_texture);
	player.setCollisionBox(50, 20, 30, 50);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::checkTileCollisions()
{
	tmm.checkTileCollision(&player);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::initAudio()
{
	audio->addMusic("sfx/Cantina.ogg", "cantina");	
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

void Level::startMusic(sf::Music music, std::string type)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::checkMusicMuted()
{
	if (!musicMuted)
	{
		if (audio->getMusic()->getStatus() == sf::SoundSource::Stopped)
		{
			audio->playMusicbyName("cantina");
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
	transFade.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
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
			fadedOut = true;
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Although named fade in, whats actually occurring is the fading out of an opaque black coloured rectangle shape, the same size as the window, giving the appearance of the level 'fading in'.
void Level::fadeInLevel()
{
	float decrAlpha = 255;

	//beginDraw();

	while (decrAlpha > 0)
	{
		// Controls the speed of fade.
		decrAlpha -= 0.1f;

		transFade.setFillColor(sf::Color(0, 0, 0, decrAlpha));
		tmm.render(window);
		window->draw(player);
		window->draw(colBox);
		//window->draw(OriginBox);
		window->draw(playerPosBox);
		window->draw(textBox);
		window->draw(text);
		window->draw(transFade);
		endDraw();

		if (decrAlpha < 150)
		{
			fadedIn = true;
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::respawnPlayer()
{
	fadedIn = false;
	player.setIsDead(false);
	player.setPosition(100, 100);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
