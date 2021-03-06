/*
 * This class controls
 *		- Creating an image for the splash screen.
 *		- Fading in/out.
 *		- Playing audio for the spalsh.
 *
 * Original @author D. Green.
 *
 * ? D. Green. 2020.
 */

 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES.
#include "Splash.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR/S & DESTRUCTOR.
Splash::Splash(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud) : Screen(hwnd, in, gs, aud)
{
	splashDelay = 0;
	waitForLogo = 0;
	fadedIn = false;
	fadedOut = false;
	switchedStates = false;

	setGameState(State::SPLASH);

	initSplashBackground();
	initTransFadeRect();
	initAudio();
}

Splash::~Splash()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS.
void Splash::handleInput(float dt)
{
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Splash::update(float dt)
{
	// If there's no music playing, play some. But only once.
	if (audio->getMusic()->getStatus() == sf::SoundSource::Stopped)
	{
		audio->playMusicbyName("splash");
		audio->getMusic()->setLoop(false);
	}

	splashDelay += dt;
	waitForLogo += dt;

	if (fadedOut)
	{
		setGameState(State::MENU);
		switchedStates = true;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Splash::render()
{
	beginDraw();
	fadeIn();

	// If we've faded in, it's time to fade out.
	if (fadedIn && !switchedStates)
	{
		fadeOut();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Splash::beginDraw()
{
	window->clear(sf::Color(255, 253, 208));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Splash::endDraw()
{
	window->display();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Splash::initSplashBackground()
{
	if (!splashLogoTexture.loadFromFile("gfx/screens/polymorphic.png"))
	{
		std::cerr << "Sorry could not load menu background image!\n";
	}

	splashBg.setSize(sf::Vector2f(378, 378));
	splashBg.setOrigin(splashBg.getSize().x / 2.0f, splashBg.getSize().y / 2.0f);
	splashBg.setPosition(sf::Vector2f(window->getSize().x / 2.0f, window->getSize().y / 2.0f));
	splashBg.setTexture(&splashLogoTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Splash::initTransFadeRect()
{
	transFade.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Splash::initAudio()
{
	audio->addMusic("sfx/splash/splash_intro.ogg", "splash");
	audio->getMusic()->setVolume(25);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Although named fade in, whats actually occurring is the fading out of an opaque cream coloured rectangle shape, the same size as the window, giving the appearance of the logo 'fading in'.
void Splash::fadeIn()
{
	// After 2 secs fade in logo splash screen
	if (splashDelay > 2.0f && !fadedIn)
	{
		float decrAlpha = 255;

		while (decrAlpha > 0)
		{
			// Controls the speed of fade.
			decrAlpha -= 0.07f;

			transFade.setFillColor(sf::Color(255, 253, 208, decrAlpha));
			window->draw(splashBg);
			window->draw(transFade);
			endDraw();

			if (decrAlpha < 1)
			{
				fadedIn = true;
				splashDelay = 0;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Although named fade out, whats actually occurring is the fading in of an opaque cream coloured rectangle shape, the same size as the window, giving the appearance of the logo 'fading out'.
void Splash::fadeOut()
{
	float incrAlpha = 0;

	while (incrAlpha < 255)
	{
		// Controls the speed of fade.
		incrAlpha += 0.07f;

		transFade.setFillColor(sf::Color(255, 253, 208, incrAlpha));
		window->draw(splashBg);
		window->draw(transFade);
		endDraw();

		if (incrAlpha > 253)
		{
			fadedOut = true;
			break;
		}
	}	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////