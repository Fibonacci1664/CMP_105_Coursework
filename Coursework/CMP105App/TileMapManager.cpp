#include "TileMapManager.h"

TileMapManager::TileMapManager()
{
	world = tileMap.getLevel();
	initTexture();
	createTileSet();
	createMap();
}

TileMapManager::~TileMapManager()
{

}

void TileMapManager::initTexture()
{
	if (!tileMap.loadTexture("gfx/tiles/env_tiles_2.png"))
	{
		std::cout << "Error loading castle environment tiles!\n";
	}
}

void TileMapManager::createTileSet()
{
	for (int i = 0; i < 4; ++i)
	{
		envTile.setSize(sf::Vector2f(64, 64));
		envTile.setCollisionBox(0, 0, 64, 64);
		envTile.setCollider(true);
		tiles.push_back(envTile);
	}

	spikesTile.setSize(sf::Vector2f(64, 64));
	spikesTile.setCollisionBox(0, 20, 64, 44);
	spikesTile.setCollider(true);
	tiles.push_back(spikesTile);

	tiles[0].setCollider(false);
	tiles[0].setTextureRect(sf::IntRect(192, 129, 63, 63));		// Num 0 tile
	tiles[1].setTextureRect(sf::IntRect(10, 0, 54, 64));			// Num 1 tile, on my env num map = 1
	tiles[2].setTextureRect(sf::IntRect(64, 0, 64, 64));		// Num 2 tile, on my env num map = 2
	tiles[3].setTextureRect(sf::IntRect(128, 0, 54, 64));		// Num 3 tile, on my env num map = 3
	tiles[4].setTextureRect(sf::IntRect(512, 256, 64, 64));		// Num 4 til,e Spike Tile, on my env num map = 44
	//tiles[5].setTextureRect(sf::IntRect(17, 34, 16, 16));		// Num 5 tile , on my mario num map = 26
	//tiles[6].setTextureRect(sf::IntRect(34, 34, 16, 16));		// Num 6 tile , on my mario num map = 27

	tileMap.setTileSet(tiles);
}

void TileMapManager::createMap()
{
	// Map dimensions.
	sf::Vector2u mapSize(15, 8);

	// Build map.
	std::vector<int> map = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 2, 3, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 2, 2, 3, 0, 0, 0, 0, 0, 0, 0,
	1, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 3,
	};

	tileMap.setTileMap(map, mapSize);
	tileMap.setPosition(sf::Vector2f(0, 0));
	tileMap.buildLevel();
}

void TileMapManager::render(sf::RenderWindow* window)
{
	tileMap.render(window);
}

void TileMapManager::checkTileCollision(Player* l_player)
{
	for (int i = 0; i < (int)world->size(); ++i)
	{
		if ((*world)[i].isCollider())
		{
			// AABB
			if (Collision::checkBoundingBox(l_player, &(*world)[i]))
			{
				l_player->collisionResponse(&(*world)[i]);
			}
		}
		else			// If were not colliding with anything then we must be in the air, so set falling to true.
		{
			l_player->setIsFalling(true);
		}
	}
}