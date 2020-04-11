#pragma once

#include"Framework/TileMap.h"
#include <SFML\Graphics\RenderWindow.hpp>
#include <iostream>
#include "Player.h"
#include "Framework/Collision.h"

class TileMapManager
{
public:
	TileMapManager();
	~TileMapManager();

	void render(sf::RenderWindow* window);
	void checkTileCollision(Player* l_player);

private:
	void initTexture();
	void createTileSet();
	void createTransparentTiles();
	void createLargeGroundTiles();
	void createSmallGroundTiles();
	void createColumnTiles();
	void createSpikeTiles();
	void createFlagTiles();
	void createTinyPlatformTile();
	void createSecretInvisibleTile();
	void createMap();

	TileMap tileMap;
	GameObject largeGroundTile;
	GameObject smallGroundTile;
	GameObject transparentTile;
	GameObject columnTile;
	GameObject spikesTile;
	GameObject flagTile;
	GameObject tinyPlatformTile;
	GameObject secretInvisbleTile;

	std::vector<GameObject> tiles;
	std::vector<GameObject>* world;
};