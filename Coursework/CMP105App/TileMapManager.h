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
	void createMap();

	TileMap tileMap;
	GameObject tile;
	std::vector<GameObject> tiles;
	std::vector<GameObject>* world;
};