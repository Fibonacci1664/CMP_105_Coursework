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
	initKeys();
}

UIPanel::~UIPanel()
{
	deleteHitPoints();
	deleteLives();
	deleteCoin();
	deleteKeys();
	deleteNumTextures();
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
	updateKeys(dt, viewsXTranslation);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::render()//sf::RenderWindow* window)
{
	drawUIIconBar();
	drawHitPoints();
	drawLives();
	drawCoin();
	drawKeys();
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

	if (!keysTextTexture.loadFromFile("gfx/level/level_UI/keys_text.png"))
	{
		std::cerr << "Sorry could not load the keys text image for the UI!\n";
	}

	if (!hpTextTexture.loadFromFile("gfx/level/level_UI/hp_text.png"))
	{
		std::cerr << "Sorry could not load UI hp text image!\n";
	}

	livesText.setSize(sf::Vector2f(78, 31.8f));
	livesText.setPosition(sf::Vector2f(20, 20));
	livesText.setTexture(&livesTextTexture);

	coinsText.setSize(sf::Vector2f(84, 31.8f));
	coinsText.setPosition(sf::Vector2f(290, 20));
	coinsText.setTexture(&coinsTextTexture);

	numOfCoinsCollected.setSize(sf::Vector2f(23.4f, 31.8f));
	numOfCoinsCollected.setPosition(sf::Vector2f(440, 20));
	numOfCoinsCollected.setTexture(numOfCoinsCollectedTextures[0]);

	keysText.setSize(sf::Vector2f(75, 31.8f));
	keysText.setPosition(sf::Vector2f(520, 20));
	keysText.setTexture(&keysTextTexture);

	hpText.setSize(sf::Vector2f(52.2f, 31.8f));
	hpText.setPosition(sf::Vector2f(window->getSize().x - 230, 20));
	hpText.setTexture(&hpTextTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::updateIconBarTextElements(float& viewsXTranslation, int& coinsCollected)
{
	livesText.setPosition(sf::Vector2f(20 + viewsXTranslation, 20));
	coinsText.setPosition(sf::Vector2f(290 + viewsXTranslation, 20));
	numOfCoinsCollected.setPosition(sf::Vector2f(440 + viewsXTranslation, 20));
	numOfCoinsCollected.setTexture(numOfCoinsCollectedTextures[coinsCollected]);
	keysText.setPosition(sf::Vector2f(520 + viewsXTranslation, 20));
	hpText.setPosition(sf::Vector2f((window->getSize().x - 230) + viewsXTranslation, 20));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::drawUIIconBar()
{
	window->draw(greyIconBar);
	window->draw(livesText);
	window->draw(coinsText);
	window->draw(numOfCoinsCollected);
	window->draw(keysText);
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
		uiHitPoints[i]->getHitPointAnimation()->setPlaying(true);
		uiHitPoints[i]->getHitPointAnimation()->setLooping(true);
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
		uiLives[i]->getHeartAnimation()->setPlaying(true);
		uiLives[i]->getHeartAnimation()->setLooping(true);
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
	coin->setPosition(sf::Vector2f(390, 20));
	coin->getCoinAnimation()->setPlaying(true);
	coin->getCoinAnimation()->setLooping(true);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::updateCoin(float& dt, float& viewsXTranslation)
{
	coin->getCoinAnimation()->animate(dt);
	coin->setTextureRect(coin->getCoinAnimation()->getCurrentFrame());
	coin->setPosition(sf::Vector2f(390 + viewsXTranslation, 20));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::drawCoin()
{
	window->draw(*coin);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::initKeys()
{
	if (!keyTexture.loadFromFile("gfx/level/key.png"))
	{
		std::cerr << "Sorry could not load key image for UI!\n";
	}

	// Create 3 keys for the UI, although in this version, the player only needs to collect 1 key to escape.
	for (int i = 0; i < 3; ++i)
	{
		Key* key = new Key;
		keys.push_back(key);

		keys[i]->setWindow(window);
		keys[i]->setSize(sf::Vector2f(64, 64));
		keys[i]->setTexture(&keyTexture);
		keys[i]->setTextureRect(keys[i]->getKeyAnimation()->getCurrentFrame());
		keys[i]->getKeyAnimation()->setPlaying(true);
		keys[i]->getKeyAnimation()->setLooping(true);
	}

	keys[0]->setPosition(sf::Vector2f(585, 0));		// Only set the position of the first key, as the player is only collecting 1 in the demo.
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::updateKeys(float& dt, float& viewsXTranslation)
{
	// Update the animations for ALL the keys even though only 1 is used.
	for (int i = 0; i < keys.size(); ++i)
	{
		keys[i]->getKeyAnimation()->animate(dt);
		keys[i]->setTextureRect(keys[i]->getKeyAnimation()->getCurrentFrame());
	}

	keys[0]->setPosition(sf::Vector2f(585 + viewsXTranslation, 0));	// Update the only one that is set and drawn.
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::drawKeys()
{
	// Only draw the keys that have been collected.
	for (int i = 0; i < keysCollected; ++i)
	{
		window->draw(*keys[i]);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::deleteHitPoints()
{
	// Delete all the UI hit points.
	for (int i = 0; i < uiHitPoints.size(); ++i)
	{
		delete uiHitPoints[i];
		uiHitPoints[i] = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::deleteLives()
{
	// Delete all the UI lives.
	for (int i = 0; i < uiLives.size(); ++i)
	{
		delete uiLives[i];
		uiLives[i] = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::deleteCoin()
{
	// Delete the UI coin.
	delete coin;
	coin = nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel:: deleteKeys()
{
	// Delete all the UI keys.
	for (int i = 0; i < keys.size(); ++i)
	{
		delete keys[i];
		keys[i] = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIPanel::deleteNumTextures()
{
	// Delete all the UI number of coins collected textures.
	for (int i = 0; i < numOfCoinsCollectedTextures.size(); ++i)
	{
		delete numOfCoinsCollectedTextures[i];
		numOfCoinsCollectedTextures[i] = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
