#include "UIPanel.h"

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

	initHitPoints();
	initGreyIconBar();
	initGreyIconBarTextElements();
}

UIPanel::~UIPanel()
{

}

void UIPanel::update(float& dt, int l_hitPointsRemaining, int l_livesRemaining, int l_coinsCollected, int l_keysCollected, float& viewsXTranslation)
{
	hitPointsRemaining = l_hitPointsRemaining;
	livesRemaining = l_livesRemaining;
	coinsCollected = l_coinsCollected;
	keysCollected = l_keysCollected;

	updateHitpoints(dt, viewsXTranslation);
	updateIconBar(viewsXTranslation);
	updateIconBarTextElements(viewsXTranslation);
}

void UIPanel::render()//sf::RenderWindow* window)
{
	drawHitPoints();
}

void UIPanel::initGreyIconBar()
{
	if (!greyIconBarTexture.loadFromFile("gfx/level/level_UI/greyIconBar.png"))
	{
		std::cerr << "Sorry could not load grey icon bar image!\n";
	}

	greyIconBar.setSize(sf::Vector2f(960, 75));
	greyIconBar.setPosition(sf::Vector2f(20, 0));
	greyIconBar.setTexture(&greyIconBarTexture);
}

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

	if (!hpTextTexture.loadFromFile("gfx/level/level_UI/hp_text.png"))
	{
		std::cerr << "Sorry could not load UI hp text image!\n";
	}

	livesText.setSize(sf::Vector2f(78, 31.8f));
	livesText.setPosition(sf::Vector2f(30, 20));
	livesText.setTexture(&livesTextTexture);

	coinsText.setSize(sf::Vector2f(84, 31.8f));
	coinsText.setPosition(sf::Vector2f(350, 20));
	coinsText.setTexture(&coinsTextTexture);

	hpText.setSize(sf::Vector2f(52.2f, 31.8f));
	hpText.setPosition(sf::Vector2f(window->getSize().x - 200, 20));
	hpText.setTexture(&hpTextTexture);
}

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
		uiHitPoints[i]->setCollisionBox(0, 0, 32, 32);
		uiHitPoints[i]->setTexture(&hitPointsTexture);
		uiHitPoints[i]->setTextureRect(uiHitPoints[i]->getHitPointAnimation()->getCurrentFrame());
	}

	uiHitPoints[0]->setPosition(sf::Vector2f(window->getSize().x - 30, 20));
	uiHitPoints[1]->setPosition(sf::Vector2f(window->getSize().x - 80, 20));
	uiHitPoints[2]->setPosition(sf::Vector2f(window->getSize().x - 130, 20));
}

void UIPanel::updateHitpoints(float& dt, float& viewsXTranslation)
{
	// Update all the hit points.
	for (int i = 0; i < 3; ++i)
	{
		uiHitPoints[i]->getHitPointAnimation()->animate(dt);
		uiHitPoints[i]->setTextureRect(uiHitPoints[i]->getHitPointAnimation()->getCurrentFrame());
	}

	uiHitPoints[0]->setPosition(sf::Vector2f((window->getSize().x - 30) + viewsXTranslation, 20));
	uiHitPoints[1]->setPosition(sf::Vector2f((window->getSize().x - 80) + viewsXTranslation, 20));
	uiHitPoints[2]->setPosition(sf::Vector2f((window->getSize().x - 130) + viewsXTranslation, 20));
}

void UIPanel::updateIconBar(float& viewsXTranslation)
{
	greyIconBar.setPosition(sf::Vector2f(20 + viewsXTranslation, 0));
}

void UIPanel::updateIconBarTextElements(float& xviewsTranslation)
{
	livesText.setPosition(sf::Vector2f(30 + xviewsTranslation, 20));
	coinsText.setPosition(sf::Vector2f(350 + xviewsTranslation, 20));
	hpText.setPosition(sf::Vector2f((window->getSize().x - 200) + xviewsTranslation, 20));
}

void UIPanel::drawHitPoints()//sf::RenderWindow* window)
{
	window->draw(greyIconBar);
	window->draw(livesText);
	window->draw(coinsText);
	window->draw(hpText);

	// Only draw the ones that are remaining.
	for (int i = 0; i < hitPointsRemaining; ++i)
	{
		window->draw(*uiHitPoints[i]);
	}	
}

void UIPanel::updateLives(float& dt)
{

}

void UIPanel::updateCoins(float& dt)
{

}

void UIPanel::updateKeyStatus(float& dt)
{

}
