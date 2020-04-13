#include "UIPanel.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTORS / DESTRUCTOR.
UIPanel::UIPanel()
{

}

UIPanel::UIPanel(sf::RenderWindow* hwnd)
{
	window = hwnd;
	hitPointsRemaining = 0;
	livesRemaining = 0;
	coinsCollected = 0;
	keysCollected = 0;

	initGreyIconBar();
	initGreyIconBarTextElements();
	initHitPoints();	
	initLives();
	initCoin();
}

UIPanel::~UIPanel()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::update(float& dt, int l_hitPointsRemaining, int l_livesRemaining, int l_coinsCollected, int l_keysCollected, float& viewsXTranslation)
{
	hitPointsRemaining = l_hitPointsRemaining;
	livesRemaining = l_livesRemaining;
	coinsCollected = l_coinsCollected;
	keysCollected = l_keysCollected;

	updateIconBar(viewsXTranslation);
	updateIconBarTextElements(viewsXTranslation, l_coinsCollected);
	updateHitpoints(dt, viewsXTranslation);	
	updateLives(dt, viewsXTranslation);
	updateCoin(dt, viewsXTranslation);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::render()//sf::RenderWindow* window)
{
	drawUIIconBar();
	drawHitPoints();
	drawLives();
	drawCoin();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::initGreyIconBar()
{
	if (!greyIconBarTexture.loadFromFile("gfx/level/level_UI/greyIconBar.png"))
	{
		std::cerr << "Sorry could not load grey icon bar image!\n";
	}

	greyIconBar.setSize(sf::Vector2f(960, 75));
	greyIconBar.setPosition(sf::Vector2f(0, 0));
	greyIconBar.setTexture(&greyIconBarTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::updateIconBar(float& viewsXTranslation)
{
	greyIconBar.setPosition(sf::Vector2f(viewsXTranslation, 0));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::initGreyIconBarTextElements()
{
	if (!livesTextTexture.loadFromFile("gfx/level/level_UI/lives_text.png"))
	{
		std::cerr << "Sorry could not load UI lives text image!\n";
	}

	if (!coinsTextTexture.loadFromFile("gfx/level/level_UI/coins_text.png"))
	{
		std::cerr << "Sorry could not load UI coins text image!\n";
	}

	// Load all the number textures for coins collected in to the vector.
	for (int i = 0; i < 7; ++i)
	{
		sf::Texture* numTexture = new sf::Texture;
		numOfCoinsCollectedTextures.push_back(numTexture);

		if (!numOfCoinsCollectedTextures[i]->loadFromFile("gfx/level/level_UI/" + std::to_string(i) + "_text.png"))
		{
			std::cerr << "Sorry could not load numbers for coins collected for UI!\n";
		}
	}

	if (!hpTextTexture.loadFromFile("gfx/level/level_UI/hp_text.png"))
	{
		std::cerr << "Sorry could not load UI hp text image!\n";
	}

	livesText.setSize(sf::Vector2f(78, 31.8f));
	livesText.setPosition(sf::Vector2f(20, 20));
	livesText.setTexture(&livesTextTexture);

	coinsText.setSize(sf::Vector2f(84, 31.8f));
	coinsText.setPosition(sf::Vector2f(300, 20));
	coinsText.setTexture(&coinsTextTexture);

	numOfCoinsCollected.setSize(sf::Vector2f(23.4f, 31.8f));
	numOfCoinsCollected.setPosition(sf::Vector2f(450, 20));
	numOfCoinsCollected.setTexture(numOfCoinsCollectedTextures[0]);

	hpText.setSize(sf::Vector2f(52.2f, 31.8f));
	hpText.setPosition(sf::Vector2f(window->getSize().x - 230, 20));
	hpText.setTexture(&hpTextTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::updateIconBarTextElements(float& viewsXTranslation, int& coinsCollected)
{
	livesText.setPosition(sf::Vector2f(20 + viewsXTranslation, 20));
	coinsText.setPosition(sf::Vector2f(300 + viewsXTranslation, 20));

	numOfCoinsCollected.setPosition(sf::Vector2f(450 + viewsXTranslation, 20));
	numOfCoinsCollected.setTexture(numOfCoinsCollectedTextures[coinsCollected]);

	hpText.setPosition(sf::Vector2f((window->getSize().x - 230) + viewsXTranslation, 20));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::drawUIIconBar()
{
	window->draw(greyIconBar);
	window->draw(livesText);
	window->draw(coinsText);
	window->draw(numOfCoinsCollected);
	window->draw(hpText);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::initHitPoints()
{
	// You can NEVER have more than 3 hit points so create them all and then only draw the amount the has been passed to the update function.
	if (!hitPointsTexture.loadFromFile("gfx/level/hitPoint.png"))
	{
		std::cerr << "Sorry could not load hit point image!\n";
	}

	// Create 3 hit points.
	for (int i = 0; i < 3; ++i)
	{
		HitPoint* hp = new HitPoint;
		uiHitPoints.push_back(hp);

		uiHitPoints[i]->setWindow(window);
		uiHitPoints[i]->setSize(sf::Vector2f(32, 32));
		uiHitPoints[i]->setTexture(&hitPointsTexture);
		uiHitPoints[i]->setTextureRect(uiHitPoints[i]->getHitPointAnimation()->getCurrentFrame());
	}

	uiHitPoints[0]->setPosition(sf::Vector2f(window->getSize().x - 70, 20));
	uiHitPoints[1]->setPosition(sf::Vector2f(window->getSize().x - 120, 20));
	uiHitPoints[2]->setPosition(sf::Vector2f(window->getSize().x - 170, 20));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::updateHitpoints(float& dt, float& viewsXTranslation)
{
	// Update all the hit points.
	for (int i = 0; i < 3; ++i)
	{
		uiHitPoints[i]->getHitPointAnimation()->animate(dt);
		uiHitPoints[i]->setTextureRect(uiHitPoints[i]->getHitPointAnimation()->getCurrentFrame());
	}

	uiHitPoints[0]->setPosition(sf::Vector2f((window->getSize().x - 70) + viewsXTranslation, 20));
	uiHitPoints[1]->setPosition(sf::Vector2f((window->getSize().x - 120) + viewsXTranslation, 20));
	uiHitPoints[2]->setPosition(sf::Vector2f((window->getSize().x - 170) + viewsXTranslation, 20));	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::drawHitPoints()
{
	// Only draw the hit points that are remaining.
	for (int i = 0; i < hitPointsRemaining; ++i)
	{
		window->draw(*uiHitPoints[i]);
	}	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::initLives()
{
	if (!livesTexture.loadFromFile("gfx/level/level_UI/hearts.png"))
	{
		std::cerr << "Sorry could not load the hearts image!\n";
	}

	for (int i = 0; i < 3; ++i)
	{
		Heart* life = new Heart;
		uiLives.push_back(life);

		uiLives[i]->setWindow(window);
		uiLives[i]->setSize(sf::Vector2f(32, 32));
		uiLives[i]->setTexture(&livesTexture);
		uiLives[i]->setTextureRect(uiLives[i]->getHeartAnimation()->getCurrentFrame());
	}

	uiLives[0]->setPosition(sf::Vector2f(110, 20));
	uiLives[1]->setPosition(sf::Vector2f(160, 20));
	uiLives[2]->setPosition(sf::Vector2f(210, 20));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::updateLives(float& dt, float& viewsXTranslation)
{
	// Update all the remaining hit points.
	for (int i = 0; i < livesRemaining; ++i)
	{
		uiLives[i]->getHeartAnimation()->animate(dt);
		uiLives[i]->setTextureRect(uiLives[i]->getHeartAnimation()->getCurrentFrame());
	}

	/*uiLives[0]->setPosition(sf::Vector2f(110, 20));
	uiLives[1]->setPosition(sf::Vector2f(160, 20));
	uiLives[2]->setPosition(sf::Vector2f(210, 20));*/

	uiLives[0]->setPosition(sf::Vector2f(110 + viewsXTranslation, 20));
	uiLives[1]->setPosition(sf::Vector2f(160 + viewsXTranslation, 20));
	uiLives[2]->setPosition(sf::Vector2f(210 + viewsXTranslation, 20));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::drawLives()
{
	// Only draw the lives that are remaining.
	for (int i = 0; i < livesRemaining; ++i)
	{
		window->draw(*uiLives[i]);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::initCoin()
{
	if (!coinTexture.loadFromFile("gfx/level/coin.png"))
	{
		std::cerr << "Sorry could not load the coin image for UI!\n";
	}

	coin = new Coin;

	coin->setWindow(window);
	coin->setSize(sf::Vector2f(32, 32));
	coin->setTexture(&coinTexture);
	coin->setTextureRect(coin->getCoinAnimation()->getCurrentFrame());
	coin->setPosition(sf::Vector2f(400, 20));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::updateCoin(float& dt, float& viewsXTranslation)
{
	coin->getCoinAnimation()->animate(dt);
	coin->setTextureRect(coin->getCoinAnimation()->getCurrentFrame());
	coin->setPosition(sf::Vector2f(400 + viewsXTranslation, 20));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::drawCoin()
{
	window->draw(*coin);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::initKeys()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::updateKeys(float& dt)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::drawKeys()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::deleteHitPoints()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::deleteLives()
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::deleteCoin()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel:: deleteKeys()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::deleteNumTextures()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
