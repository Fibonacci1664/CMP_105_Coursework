// INCLUDES.
#include "GameOver.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR/S & DESTRUCTOR.
GameOver::GameOver(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud) : Screen(hwnd, in, gs, aud)
{
	fadedIn = false;
	fadedOut = false;
	switchedStates = false;

	initGameOverRect();
	initTransFadeRect();
	initAudio();
}

GameOver::~GameOver()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS.
void GameOver::handleInput(float dt)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GameOver::update(float dt)
{
	// The sweet sounds of death!
	if (audio->getMusic()->getStatus() == sf::SoundSource::Stopped)
	{
		// Play some somber death music.
		audio->playMusicbyName("gameover");
	}

	/*
	 * If the lovely 'you died' graphic has faded out then switch state back to level for a respawn.
	 * CAREFUL YOU STILL HAVE 2 RENDER CALLS FROM HERE THOUGH!
	 */
	if (fadedOut)
	{
		setGameState(State::MENU);
		switchedStates = true;
		fadedIn = false;
		audio->stopAllMusic();

		if (audio->getMusic()->getStatus() == sf::SoundSource::Stopped)
		{
			audio->playMusicbyName("splash");
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GameOver::render()
{
	beginDraw();

	/*
	 * Lots of bool checks here to control what gets displayed when, as even though we switch states
	 * in update, we still have another 2 render calls here before we actually switch back to level.
	 * without these bool checks we get wierd things happening like the 'you died' text fading in, fading out
	 * then fading in again on the next render call, which we obviously dont want.
	 */
	if (!fadedIn && !switchedStates)
	{
		fadeIn();
	}


	// If we've faded in, it's time to fade out to black.
	if (fadedIn && !switchedStates)
	{
		fadeOut();
	}

	/*
	 * Not ideal to have this here, as its nothing to do with drawing, but this is the last thing to run before
	 * switching back to level after displaying the fade in, fade out text, and we need to leave things on false ready
	 * for when we die again so the logic above works as it should.
	 */
	if (switchedStates)
	{
		switchedStates = false;
		fadedOut = false;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GameOver::beginDraw()
{
	//window->clear(sf::Color(255, 253, 208));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GameOver::endDraw()
{
	window->display();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GameOver::initGameOverRect()
{
	if (!gameOverTexture.loadFromFile("gfx/text/game_over.png"))
	{
		std::cerr << "Sorry could not game over text image!\n";
	}

	// orign x = 372 orig y = 83.
	gameOverRect.setSize(sf::Vector2f(334, 56.7f));
	gameOverRect.setOrigin(gameOverRect.getSize().x / 2.0f, gameOverRect.getSize().y / 2.0f);
	gameOverRect.setPosition(sf::Vector2f(window->getSize().x / 2.0f, window->getSize().y / 2.0f));
	gameOverRect.setTexture(&gameOverTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GameOver::initTransFadeRect()
{
	transFade.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GameOver::initAudio()
{
	audio->addMusic("sfx/gameover/game_over.ogg", "gameover");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Although named fade in, whats actually occurring is the fading out of an opaque black coloured rectangle shape, the same size as the window, giving the appearance of the 'you died' text 'fading in'.
void GameOver::fadeIn()
{
	float decrAlpha = 255;

	while (decrAlpha > 1)
	{
		// Controls the speed of fade.
		decrAlpha -= 0.07f;

		transFade.setFillColor(sf::Color(0, 0, 0, decrAlpha));
		window->draw(gameOverRect);
		window->draw(transFade);
		endDraw();
	}

	fadedIn = true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Although named fade out, whats actually occurring is the fading in of an opaque black coloured rectangle shape, the same size as the window, giving the appearance of the 'you died' text 'fading out'.
void GameOver::fadeOut()
{
	float incrAlpha = 0;

	while (incrAlpha < 253)
	{
		// Controls the speed of fade.
		incrAlpha += 0.07f;

		transFade.setFillColor(sf::Color(0, 0, 0, incrAlpha));
		window->draw(gameOverRect);
		window->draw(transFade);
		endDraw();
	}

	fadedOut = true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////