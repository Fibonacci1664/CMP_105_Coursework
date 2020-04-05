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
	// Element [0] : Transparent tile.
	transparentTile.setSize(sf::Vector2f(64, 64));
	transparentTile.setCollider(false);
	tiles.push_back(transparentTile);

	// Elements [1] - [8] : Large ground tiles.
	for (int i = 0; i < 9; ++i)
	{
		groundTile.setSize(sf::Vector2f(64, 64));
		groundTile.setCollisionBox(0, 0, 64, 64);
		groundTile.setCollider(true);
		tiles.push_back(groundTile);
	}

	// Elements [9] - [18] : Smaller ground tiles.
	for (int i = 0; i < 11; ++i)
	{
		groundTile.setSize(sf::Vector2f(64, 64));
		groundTile.setCollisionBox(5, 0, 59, 32);
		groundTile.setCollider(true);
		tiles.push_back(groundTile);
	}

	// Elements [19] - [24] : Column tiles.
	for (int i = 0; i < 7; ++i)
	{
		columnTile.setSize(sf::Vector2f(64, 64));
		columnTile.setCollisionBox(10, 0, 44, 64);
		columnTile.setCollider(true);
		tiles.push_back(groundTile);
	}

	// Element [25] : Spikes.
	spikesTile.setSize(sf::Vector2f(64, 64));
	spikesTile.setCollisionBox(0, 32, 64, 44);
	spikesTile.setCollider(true);
	tiles.push_back(spikesTile);

	// Transprent tile.
	tiles[0].setTextureRect(sf::IntRect(576, 320, 64, 64));		// Num 0 tile, bottom right corner.			DONE

	// Large ground tiles.
	tiles[1].setTextureRect(sf::IntRect(10, 0, 64, 64));		// Num 1 tile, on my env num map = 1		DONE
	tiles[2].setTextureRect(sf::IntRect(64, 0, 64, 64));		// Num 2 tile, on my env num map = 2		DONE
	tiles[3].setTextureRect(sf::IntRect(128, 0, 64, 64));		// Num 3 tile, on my env num map = 3		DONE
	tiles[4].setTextureRect(sf::IntRect(320, 0, 64, 64));		// Num 4 tile, on my env num map = 6		DONE
	tiles[5].setTextureRect(sf::IntRect(384, 0, 64, 64));		// Num 5 tile, on my env num map = 7		DONE
	tiles[6].setTextureRect(sf::IntRect(10, 64, 64, 64));		// Num 6 tile, on my env num map = 11		DONE
	tiles[7].setTextureRect(sf::IntRect(64, 64, 64, 64));		// Num 7 tile, on my env num map = 12		DONE
	tiles[8].setTextureRect(sf::IntRect(128, 64, 64, 64));		// Num 8 tile, on my env num map = 13		DONE

	// Smaller ground tiles & undersides.
	tiles[9].setTextureRect(sf::IntRect(10, 128, 64, 64));		// Num 9 tile, on my env num map = 21		DONE
	tiles[10].setTextureRect(sf::IntRect(64, 128, 64, 64));		// Num 10 tile, on my env num map = 22		DONE
	tiles[11].setTextureRect(sf::IntRect(128, 128, 64, 64));	// Num 11 tile, on my env num map = 23		DONE
	tiles[12].setTextureRect(sf::IntRect(320, 64, 64, 64));		// Num 12 tile, on my env num map = 16		DONE
	tiles[13].setTextureRect(sf::IntRect(384, 64, 64, 64));		// Num 13 tile, on my env num map = 17		DONE
	tiles[14].setTextureRect(sf::IntRect(10, 256, 64, 64));		// Num 14 tile, on my env num map = 36		DONE
	tiles[15].setTextureRect(sf::IntRect(64, 256, 64, 64));		// Num 15 tile, on my env num map = 37		DONE
	tiles[16].setTextureRect(sf::IntRect(128, 256, 64, 64));	// Num 16 tile, on my env num map = 38		DONE
	tiles[17].setTextureRect(sf::IntRect(384, 256, 64, 64));	// Num 17 tile, on my env num map = 42		DONE
	tiles[18].setTextureRect(sf::IntRect(448, 256, 64, 64));	// Num 18 tile, on my env num map = 43		DONE

	// Column tiles, White column
	tiles[19].setTextureRect(sf::IntRect(512, 0, 64, 64));		// Num 19 tile, on my env num map = 9		DONE
	tiles[20].setTextureRect(sf::IntRect(512, 64, 64, 64));		// Num 20 tile, on my env num map = 19		DONE
	tiles[21].setTextureRect(sf::IntRect(512, 128, 64, 64));	// Num 21 tile, on my env num map = 25		DONE

	// Black column
	tiles[22].setTextureRect(sf::IntRect(576, 0, 64, 64));		// Num 22 tile, on my env num map = 10		DONE
	tiles[23].setTextureRect(sf::IntRect(576, 64, 64, 64));		// Num 23 tile, on my env num map = 20		DONE
	tiles[24].setTextureRect(sf::IntRect(576, 128, 64, 64));	// Num 24 tile, on my env num map = 26		DONE

	// Spike tile.
	tiles[25].setTextureRect(sf::IntRect(512, 256, 64, 64));	// Num 25 tile, on my env num map = 44		DONE

	tileMap.setTileSet(tiles);
}

void TileMapManager::createMap()
{
	// Map dimensions.
	sf::Vector2u mapSize(15, 8);

	// Build map.
	std::vector<int> map = {
	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	24,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	14,	15,	16,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	22,	0,	0,	0,	0,	0,	0,	0,	19,	0,	0,	0,	0,	0,	0,
	23,	0,	0,	0,	0,	0,	19,	0,	20,	0,	0,	0,	0,	0,	0,
	24,	0,	0,	0,	19,	0,	20,	0,	20,	0,	0,	0,	0,	0,	0,
	1,	2,	3,	25,	21,	25,	21,	25,	21,	25,	25,	25,	25,	25,	25,
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