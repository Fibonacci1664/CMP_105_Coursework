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
}

void UIPanel::render()//sf::RenderWindow* window)
{
	drawHitPoints();
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

	uiHitPoints[0]->setPosition(sf::Vector2f(window->getSize().x - 50, 25));
	uiHitPoints[1]->setPosition(sf::Vector2f(window->getSize().x - 100, 25));
	uiHitPoints[2]->setPosition(sf::Vector2f(window->getSize().x - 150, 25));
}

void UIPanel::updateHitpoints(float& dt, float& viewsXTranslation)
{
	// Update all the hit points.
	for (int i = 0; i < 3; ++i)
	{
		uiHitPoints[i]->getHitPointAnimation()->animate(dt);
		uiHitPoints[i]->setTextureRect(uiHitPoints[i]->getHitPointAnimation()->getCurrentFrame());
	}

	uiHitPoints[0]->setPosition(sf::Vector2f((window->getSize().x - 50) + viewsXTranslation, 25));
	uiHitPoints[1]->setPosition(sf::Vector2f((window->getSize().x - 100) + viewsXTranslation, 25));
	uiHitPoints[2]->setPosition(sf::Vector2f((window->getSize().x - 150) + viewsXTranslation, 25));
}

void UIPanel::drawHitPoints()//sf::RenderWindow* window)
{
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
