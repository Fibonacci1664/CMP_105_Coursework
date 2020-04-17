/*
 * This class controls
 *		- Loading the texture that the tile map is created form.
 *		- Create a vector of tiles.
 *		- Create a tile map using the tiles.
 *
 * Original @author D. Green.
 *
 * © D. Green. 2020.
 */

 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES.
#include "TileMapManager.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTORS & DESTRUCTOR.
TileMapManager::TileMapManager()
{
	initTexture();
	createTileSet();
	createMap();
	world = tileMap.getLevel();
}

TileMapManager::~TileMapManager()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS.
void TileMapManager::initTexture()
{
	if (!tileMap.loadTexture("gfx/tiles/env_tiles_2.png"))
	{
		std::cout << "Error loading castle environment tiles!\n";
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TileMapManager::createTileSet()
{
	createTransparentTiles();
	createLargeGroundTiles();
	createSmallGroundTiles();
	createColumnTiles();
	createSpikeTiles();
	createFlagTiles();
	createTinyPlatformTile();
	createSecretInvisibleTile();


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

	//// Column tiles, White column
	tiles[19].setTextureRect(sf::IntRect(512, 0, 64, 64));		// Num 19 tile, on my env num map = 9		DONE
	tiles[20].setTextureRect(sf::IntRect(512, 64, 64, 64));		// Num 20 tile, on my env num map = 19		DONE
	tiles[21].setTextureRect(sf::IntRect(512, 128, 64, 64));	// Num 21 tile, on my env num map = 25		DONE

	// Black column
	tiles[22].setTextureRect(sf::IntRect(576, 0, 64, 64));		// Num 22 tile, on my env num map = 10		DONE
	tiles[23].setTextureRect(sf::IntRect(576, 64, 64, 64));		// Num 23 tile, on my env num map = 20		DONE
	tiles[24].setTextureRect(sf::IntRect(576, 128, 64, 64));	// Num 24 tile, on my env num map = 26		DONE

	// Spike tile.
	tiles[25].setTextureRect(sf::IntRect(512, 256, 64, 64));	// Num 25 tile, on my env num map = 44		DONE

	// Flag tile
	tiles[26].setTextureRect(sf::IntRect(384, 320, 64, 64));	// Num 26 tile, on my env num map = 51		DONE

	// Tiny platform tile.
	tiles[27].setTextureRect(sf::IntRect(320, 256, 64, 64));	// Num 27 tile, on my env num map = 41		DONE

	// Secret invisible tile.
	tiles[28].setTextureRect(sf::IntRect(512, 321, 64, 63));	// Num 28 tile, on my env num map = 0, Just after 53, before the corner.		DONE

	tileMap.setTileSet(tiles);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TileMapManager::createTransparentTiles()
{
	// Element [0] : Transparent tile.
	transparentTile.setSize(sf::Vector2f(64, 64));
	transparentTile.setCollider(false);
	tiles.push_back(transparentTile);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TileMapManager::createLargeGroundTiles()
{
	// Elements [1] - [8] : Large ground tiles.
	for (int i = 0; i < 8; ++i)
	{
		largeGroundTile.setSize(sf::Vector2f(64, 64));
		largeGroundTile.setCollisionBox(0, 0, 64, 64);
		largeGroundTile.setCollider(true);
		largeGroundTile.setIndex(i + 1);
		/*largeGroundTile.setOutlineColor(sf::Color::Magenta);
		largeGroundTile.setOutlineThickness(1.0f);*/
		tiles.push_back(largeGroundTile);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TileMapManager::createSmallGroundTiles()
{
	// Elements [9] - [18] : Smaller ground tiles.
	for (int i = 0; i < 10; ++i)
	{
		smallGroundTile.setSize(sf::Vector2f(64, 64));
		smallGroundTile.setCollisionBox(5, 0, 59, 32);
		smallGroundTile.setCollider(true);
		smallGroundTile.setIndex(i + 9);
		tiles.push_back(smallGroundTile);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TileMapManager::createColumnTiles()
{
	// Elements [19] - [24] : Column tiles.
	for (int i = 0; i < 6; ++i)
	{
		columnTile.setSize(sf::Vector2f(64, 64));
		columnTile.setCollisionBox(12, 0, 40, 64);
		columnTile.setCollider(true);
		columnTile.setIndex(i + 19);
		/*columnTile.setOutlineColor(sf::Color::Magenta);
		columnTile.setOutlineThickness(1.0f);*/
		tiles.push_back(columnTile);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TileMapManager::createSpikeTiles()
{
	// Element [25] : Spikes.
	spikesTile.setSize(sf::Vector2f(64, 64));
	spikesTile.setCollisionBox(0, 10, 64, 44);
	spikesTile.setCollider(true);
	spikesTile.setIndex(25);							// THIS IS A UNIQUE IDENTIFIER, its index pos. From research i could do this through std algo to retrieve the index pos, but dont know enough yet, plus this just seemed simpler.
	/*spikesTile.setOutlineColor(sf::Color::Magenta);
	spikesTile.setOutlineThickness(1.0f);*/
	tiles.push_back(spikesTile);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TileMapManager::createFlagTiles()
{
	// Element [26] : Flag.
	flagTile.setSize(sf::Vector2f(64, 64));
	/*flagTile.setOutlineColor(sf::Color::Magenta);
	flagTile.setOutlineThickness(1.0f);*/
	tiles.push_back(flagTile);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TileMapManager::createTinyPlatformTile()
{
	tinyPlatformTile.setSize(sf::Vector2f(64, 64));
	tinyPlatformTile.setCollisionBox(10, 0, 44, 32);
	tinyPlatformTile.setCollider(true);
	tinyPlatformTile.setIndex(27);
	tiles.push_back(tinyPlatformTile);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TileMapManager::createSecretInvisibleTile()
{
	secretInvisbleTile.setSize(sf::Vector2f(64, 64));
	secretInvisbleTile.setCollisionBox(12, 0, 12, 63);
	secretInvisbleTile.setCollider(true);
	secretInvisbleTile.setIndex(28);
	tiles.push_back(secretInvisbleTile);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TileMapManager::createMap()
{
	// Map dimensions.
	sf::Vector2u mapSize(45, 8);

	// Build map.
	std::vector<int> map = {
	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,
	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	23,
	24,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	14,	15,	16,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	3,	0,	0,	0,	19,	0,	0,	23,
	14,	15,	16,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	17,	18,	0,	9,	11,	0,	0,	0,	20,	0,	0,	23,
	22, 0,	0,	0,	0,	0,	0,	0,	19,	0,	1,	3,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	2,	3,	0,	0,	0,	0,	0,	0,	0,	0,	0,	20,	0,	0,	23,
	23, 0,	0,	0,	0,	0,	19,	0,	20,	0,	9,	11,	0,	17,	18,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	3,	0,	6,	7,	8,	0,	0,	0,	0,	0,	0,	0,	0,	0,	20,	0,	0,	23,
	24, 0,	26,	0,	19,	0,	20,	0,	20,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	17,	18,	0,	9,	11,	0,	9,	10,	11,	0,	27,	0,	0,	0,	0,	0,	0,	28,	20,	0,	0,	24,
	1,	2,	3,	25,	21,	25,	21,	25,	21,	25,	25,	25,	25,	25,	25, 1,	2,	2,	2,	2,	2,	3,	25,	25,	25,	25,	25, 25,	25,	25,	25,	25,	25, 25,	25,	25,	1,	3, 25,	25,	25,	21,	1,	2,	3,
	};

	tileMap.setTileMap(map, mapSize);
	tileMap.setPosition(sf::Vector2f(0, 0));
	tileMap.buildLevel();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TileMapManager::render(sf::RenderWindow* window)
{
	tileMap.render(window);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////