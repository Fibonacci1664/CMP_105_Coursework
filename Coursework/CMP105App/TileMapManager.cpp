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
	if (!tileMap.loadTexture("gfx/tiles/marioTiles_3.png"))
	{
		std::cout << "Error loading mario tiles!\n";
	}
}

void TileMapManager::createTileSet()
{
	for (int i = 0; i < 7; ++i)
	{
		tile.setSize(sf::Vector2f(32, 32));
		tile.setCollisionBox(0, 0, 32, 32);
		tile.setCollider(true);
		tiles.push_back(tile);
	}

	tiles[0].setCollider(false);
	tiles[0].setTextureRect(sf::IntRect(187, 51, 16, 16));		// Num 0 tile
	tiles[1].setTextureRect(sf::IntRect(0, 0, 16, 16));			// Num 1 tile, on my mario num map = 1
	tiles[2].setTextureRect(sf::IntRect(17, 0, 16, 16));		// Num 2 tile , on my mario num map = 2
	tiles[3].setTextureRect(sf::IntRect(34, 0, 16, 16));		// Num 3 tile , on my mario num map = 3
	tiles[4].setTextureRect(sf::IntRect(0, 34, 16, 16));		// Num 4 tile , on my mario num map = 25
	tiles[5].setTextureRect(sf::IntRect(17, 34, 16, 16));		// Num 5 tile , on my mario num map = 26
	tiles[6].setTextureRect(sf::IntRect(34, 34, 16, 16));		// Num 6 tile , on my mario num map = 27

	tileMap.setTileSet(tiles);
}

void TileMapManager::createMap()
{
	// Map dimensions.
	sf::Vector2u mapSize(30, 16);

	// Build map.
	std::vector<int> map = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 0, 0, 4, 6, 0, 0, 0, 0, 1, 3, 0, 0, 4, 6,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 4, 6, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 3, 0, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 1, 3, 0, 0, 4, 6, 0, 0, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 4, 6, 0,
	1, 2, 2, 2, 2, 2, 2, 3, 4, 6, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3,
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