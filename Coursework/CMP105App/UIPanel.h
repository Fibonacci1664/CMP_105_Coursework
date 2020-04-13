#pragma once
#include <vector>
#include "Screen.h"
#include "HitPoint.h"
#include "Heart.h"
#include "Coin.h"
#include "Key.h"

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
	void initCoin();
	void initKeys();

	// Update stuff on the UI panel.
	void updateIconBar(float& viewsXTranslation);
	void updateIconBarTextElements(float& viewsXTranslation, int& l_coinsCollected);
	void updateHitpoints(float& dt, float& viewsXTranslation);
	void updateLives(float& dt, float& viewsXTranslation);
	void updateCoin(float& dt, float& viewsXTranslation);
	void updateKeys(float& dt, float& viewsXTranslation);

	// Draw stuff.
	void drawUIIconBar();
	void drawHitPoints();
	void drawLives();
	void drawCoin();
	void drawKeys();

	// Delete stuff.
	// DELETE ALL THE WEHRE 'NEW' HAS BEEN USED.
	void deleteHitPoints();
	void deleteLives();
	void deleteCoin();
	void deleteKeys();
	void deleteNumTextures();

	sf::RectangleShape greyIconBar;
	sf::Texture greyIconBarTexture;

	sf::RectangleShape livesText;
	sf::Texture livesTextTexture;

	std::vector<Heart*> uiLives;
	sf::Texture livesTexture;

	sf::RectangleShape coinsText;
	sf::Texture coinsTextTexture;

	sf::RectangleShape numOfCoinsCollected;
	std::vector<sf::Texture*> numOfCoinsCollectedTextures;	

	Coin* coin;
	sf::Texture coinTexture;

	sf::RectangleShape hpText;
	sf::Texture hpTextTexture;

	std::vector<HitPoint*> uiHitPoints;
	sf::Texture hitPointsTexture;

	sf::RectangleShape keysText;
	sf::Texture keysTextTexture;

	std::vector<Key*> keys;
	sf::Texture keyTexture;

	sf::RenderWindow* window;

	int hitPointsRemaining;
	int livesRemaining;
	int coinsCollected;
	int keysCollected;
};

