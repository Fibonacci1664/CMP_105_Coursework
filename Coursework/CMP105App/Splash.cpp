#include "Splash.h"

Splash::Splash(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud) : Screen(hwnd, in, gs, aud)
{
	splashDelay = 0;
	waitForLogo = 0;
	fadedIn = false;

	setGameState(State::SPLASH);

	initSplashBackground();
	initTransFadeRect();
}

Splash::~Splash()
{

}

void Splash::handleInput(float dt)
{
	
}

void Splash::update(float dt)
{
	splashDelay += dt;
	waitForLogo += dt;

	if (waitForLogo > 10)
	{
		setGameState(State::MENU);
	}
}

void Splash::render()
{
	beginDraw();

	if (splashDelay > 2 && !fadedIn)
	{
		float decr = 255;

		while (decr > 0)
		{
			decr -= 0.05;

			transFade.setFillColor(sf::Color(255, 253, 208, decr));
			window->draw(splashBg);
			window->draw(transFade);
			endDraw();

			if (decr < 1)
			{
				fadedIn = true;
				splashDelay = 0;
			}
		}	
	}
}

void Splash::beginDraw()
{
	//window->clear(sf::Color(255, 253, 208));
}

void Splash::endDraw()
{
	window->display();
}

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

void Splash::initTransFadeRect()
{
	transFade.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
}