#pragma once
#include <vector>
#include "Screen.h"
#include "HitPoint.h"
#include "Heart.h"

class UIPanel : public GameObject
{
public:
	UIPanel();
	UIPanel(sf::RenderWindow* hwnd);
	~UIPanel();

	void update(float& dt, int l_hitPointsRemaining, int l_livesRemaining, int l_coinsCollected, int l_keysCollected, float& viewsXTranslation);
	void render();

private:
	// Init stuff.
	void initGreyIconBar();
	void initGreyIconBarTextElements();
	void initHitPoints();
	void initLives();
	void initCoins();

	// Update stuff on the UI panel.
	void updateHitpoints(float& dt, float& viewsXTranslation);
	void updateIconBar(float& viewsXTranslation);
	void updateIconBarTextElements(float& xviewsTranslation);

	void updateLives(float& dt);
	void updateCoins(float& dt);
	void updateKeyStatus(float& dt);

	// Draw stuff.
	void drawHitPoints();//sf::RenderWindow* window);

	sf::RectangleShape greyIconBar;
	sf::Texture greyIconBarTexture;

	sf::RectangleShape livesText;
	sf::Texture livesTextTexture;
	sf::RectangleShape coinsText;
	sf::Texture coinsTextTexture;
	sf::RectangleShape hpText;
	sf::Texture hpTextTexture;

	std::vector<HitPoint*> uiHitPoints;
	sf::Texture hitPointsTexture;

	/*std::vector<Heart> uiLives;
	sf::Texture livesTexture;*/

	sf::RenderWindow* window;

	int hitPointsRemaining;
	int livesRemaining;
	int coinsCollected;
	int keysCollected;
};

