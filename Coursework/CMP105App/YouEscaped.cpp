// INCLUDES.
#include "YouEscaped.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR/S & DESTRUCTOR.
YouEscaped::YouEscaped(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud) : Screen(hwnd, in, gs, aud)
{
	fadedIn = false;
	fadedOut = false;
	switchedStates = false;
	playedMusic = false;
	showEscapedScreenTime = 0;

	initYouEscapedRect();
	initTransFadeRect();
	initAudio();
}

YouEscaped::~YouEscaped()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS.
void YouEscaped::handleInput(float dt)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void YouEscaped::update(float dt)
{
	showEscapedScreenTime += dt;

	// If we've already played the winning jingle then dont play it again.
	if (playedMusic)
	{
		audio->stopAllMusic();
	}	
	else if(audio->getMusic()->getStatus() == sf::SoundSource::Stopped)	// The sweet sounds of victory!
	{
		// Play some cheery winning jingle.
		audio->getMusic()->setLoop(false);
		audio->playMusicbyName("escaped");
		playedMusic = true;
	}

	// Fade out back to menu.
	if (fadedOut)
	{
		setGameState(State::MENU);
		switchedStates = true;
		fadedIn = false;
		audio->stopAllMusic();

		if (audio->getMusic()->getStatus() == sf::SoundSource::Stopped)
		{
			audio->getMusic()->setLoop(true);
			audio->playMusicbyName("ambience");
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void YouEscaped::render()
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

void YouEscaped::beginDraw()
{
	//window->clear(sf::Color(255, 253, 208));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void YouEscaped::endDraw()
{
	window->display();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void YouEscaped::initYouEscapedRect()
{
	if (!youEscapedTexture.loadFromFile("gfx/screens/daybreak_final.png"))
	{
		std::cerr << "Sorry could not daybreak image!\n";
	}

	youEscapedRect.setSize(sf::Vector2f(960, 540));
	youEscapedRect.setTexture(&youEscapedTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void YouEscaped::initTransFadeRect()
{
	transFade.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void YouEscaped::initAudio()
{
	audio->addMusic("sfx/escaped/escaped.ogg", "escaped");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Although named fade in, whats actually occurring is the fading out of an opaque black coloured rectangle shape, the same size as the window, giving the appearance of the 'you died' text 'fading in'.
void YouEscaped::fadeIn()
{
	float decrAlpha = 255;

	while (decrAlpha > 1)
	{
		// Controls the speed of fade.
		decrAlpha -= 0.07f;

		transFade.setFillColor(sf::Color(0, 0, 0, decrAlpha));
		window->draw(youEscapedRect);
		window->draw(transFade);
		endDraw();
	}

	fadedIn = true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Although named fade out, whats actually occurring is the fading in of an opaque black coloured rectangle shape, the same size as the window, giving the appearance of the 'you died' text 'fading out'.
void YouEscaped::fadeOut()
{
	float incrAlpha = 0;

	while (incrAlpha < 253)
	{
		// Controls the speed of fade.
		incrAlpha += 0.07f;

		transFade.setFillColor(sf::Color(0, 0, 0, incrAlpha));
		window->draw(youEscapedRect);
		window->draw(transFade);
		endDraw();
	}

	fadedOut = true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////