#include "TileMap.h"

TileMap::TileMap()
{
}

TileMap::TileMap(string tilesName, string animName)
{
	mapsNames = { "map_1-1.txt","map_1-2.txt","map_1-2_over.txt", "map_1-3.txt", "map_1-4.txt" };
	currMap = 0;
	mapChanged = false;
	
	loadMapFromFile();

	TileTexture.loadFromFile(tilesName);
	Tiles.setTexture(TileTexture);

	AnimTexture.loadFromFile(animName);
	questionMarksSprite.setTexture(AnimTexture);
	questionMarksSprite.setTextureRect(IntRect(0, 0, 0, 0));
	lavaSprite.setTexture(AnimTexture);
	lavaSprite.setTextureRect(IntRect(0, 0, 0, 0));

	itemsTexture.loadFromFile("Items.png");
	coinsSprite.setTexture(itemsTexture);
	mushroomSprite.setTexture(itemsTexture);
	inizSprite.setTexture(itemsTexture);
	starSprite.setTexture(itemsTexture);
	castleFlagSprite.setTexture(itemsTexture);
	axeSprite.setTexture(itemsTexture);
	princessSprite.setTexture(itemsTexture);
	coinsSprite.setTextureRect(IntRect(0, 84, 0, 0));
	mushroomSprite.setTextureRect(IntRect(128, 148, 0, 0));
	starSprite.setTextureRect(IntRect(128,212,0,0));
	castleFlagSprite.setTextureRect(IntRect(0,148,0,0));
	axeSprite.setTextureRect(IntRect(0,212,0,0));
	BackgroundOverworldTexture.loadFromFile("background.png");
	BackgroundUnderworldTexture.loadFromFile("Underground.png");
	Background.setTexture(BackgroundOverworldTexture);

	coinBuffer.loadFromFile("coin.wav");
	coinSound.setBuffer(coinBuffer);

	powerupAppearsBuffer.loadFromFile("powerup_appears.wav");
	powerupAppearsSound.setBuffer(powerupAppearsBuffer);

	powerupBuffer.loadFromFile("powerup.wav");
	powerupSound.setBuffer(powerupBuffer);

	blockBreakBuffer.loadFromFile("breakblock.wav");
	blockBreakSound.setBuffer(blockBreakBuffer);

	endingMapTime = 0.0f;
	movingPlatformsW2 = {Vector2f(4480,150), Vector2f(4960,150)};
	movingPlatformsW3 = { Vector2f(1863,400),  Vector2f(3008,400),  Vector2f(3255,400), Vector2f(4696,400) };
	platform.setTexture(itemsTexture);
	platform.setTextureRect(platformRect);
	changePlatforms = true;
}

void TileMap::update(RenderWindow& W, Mario& CMario, float deltaTime)
{
	W.draw(Background);

	for (auto i = 0; i < Map.size(); i++)
	{
		for (auto j = 0; j < Map[i].size(); j++)
		{
			Tiles.setPosition(static_cast<float>(j * 32), static_cast<float>(i * 32));
			position = Tiles.getPosition();

			if (Map[i][j])
			{
				if (Map[i][j] > 0 && Map[i][j] <= 48 && Map[i][j] != COIN_TILE_CODE && Map[i][j] != MUSHROOM_TILE_CODE)
				{
					RectTiles.top = 0;

					if ((Map[i][j] == 3 || Map[i][j] == 2 || Map[i][j] == STAR_BRICK_CODE || Map[i][j] == LIVE_UP_CODE || 
						Map[i][j] == BRICK_MUSHROOM_TILE_CODE))
					{
						RectTiles.left = 32;
					}
					else 
					{
						RectTiles.left = (Map[i][j] - 1) * 32;
					}
				}

				if (Map[i][j] == COIN_TILE_CODE || Map[i][j] == MUSHROOM_TILE_CODE)
				{
					questionMarksDrawing(W, i, j);
				}

				else if (Map[i][j] >= 49 && Map[i][j] <= 88 && Map[i][j] != LAVA_CODE)
				{
					RectTiles.left = (Map[i][j] - 49) * 32;
					RectTiles.top = 32;
				}

				else if(Map[i][j] == LAVA_CODE)
				{
					lavaDrawing(W, i, j);
				}
				
				else if (Map[i][j] == COIN_CODE)
				{
					coinsDrawing(W, i, j);
				}
				
				else if (Map[i][j] == MUSHROOM_CODE)
				{
					mushroomSprite.setTextureRect(IntRect(128, 148, 32, 32));
					mushroomSprite.setPosition(static_cast<float>(j * 32), static_cast<float>(i * 32));
					//mushroomSprites.emplace_back(make_unique<Sprite>(mushroomSprite));
					mushroomSprites.emplace_back(mushroomSprite);

				}
				
				else if (Map[i][j] == INIZ_UP_CODE)
				{
					inizSprite.setTextureRect(IntRect(160, 148, 32, 32));
					inizSprite.setPosition(static_cast<float>(j * 32), static_cast<float>(i * 32));
					W.draw(inizSprite);
				}

				else if (Map[i][j] == STAR_CODE)
				{
					starsDrawing(W, i, j);
				}

				else if (Map[i][j] == CASTLE_FLAG_CODE)
				{
					castleFlagDrawing(W, i, j);
				}

				else if (Map[i][j] == AXE_CODE)
				{
					axeSprite.setTextureRect(IntRect(0, 212, 32, 32));
					axeSprite.setPosition(static_cast<float>(j * 32), static_cast<float>(i * 32));
				}
				
				Tiles.setTextureRect(RectTiles); //tutaj dopiero ustawiam TextureRect

			}
			else
				Tiles.setTextureRect(IntRect(96, 352, 32, 32)); //ustawiam teksture dla zera

			if (nLevelWidth < j * 32.0f)
				nLevelWidth = j * 32.0f;
			if (nLevelHeight < i * 32.0f)
				nLevelHeight = i * 32.0f;

			if(CMario.getIfGameCompleted())
			{
				removeBridge(i, j);
			}


			if(Map[i][j] == CASTLE_FLAG_CODE)
				W.draw(castleFlagSprite);
			else if (Map[i][j] == MUSHROOM_CODE)
				W.draw(mushroomSprite);
			else if(Map[i][j] != MUSHROOM_CODE && Map[i][j] != COIN_CODE && Map[i][j] != INIZ_UP_CODE && Map[i][j] != STAR_CODE && Map[i][j] != LAVA_CODE && Map[i][j] != AXE_CODE)
				W.draw(Tiles);
			
			if (currMap != 2 && (Map[i][j] == COIN_TILE_CODE || Map[i][j] == MUSHROOM_TILE_CODE))
				W.draw(questionMarksSprite);
			if (currMap == 4 && Map[i][j] == LAVA_CODE)
				W.draw(lavaSprite);
			W.draw(axeSprite);

		}
	}

	if (currMap == 1 || currMap == 3)
	{
		initializePlatforms();
		movingPlatforms(W);
	}
	else if (currMap == 4)
		princessDrawing(W);
}

void TileMap::draw(RenderWindow& W)
{
}

int TileMap::getTile(float x, float y)
{
	auto i = 0;
	auto j = 0;
	if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
	{
		i = static_cast<int>(x / 32);
		j = static_cast<int>(y / 32);
	}

	return Map[j][i];
}

void TileMap::setTile(float x, float y, const int tileCode)
{
	auto i = 0;
	auto j = 0;
	if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
	{
		i = static_cast<int>(x / 32);
		j = static_cast<int>(y / 32);
	}
	
	Map[j - 1][i] = tileCode;
}

void TileMap::loadMapFromFile()
{
	ifstream openfile(mapsNames[currMap]);
	if (!openfile.is_open())
		throw runtime_error("Can't load file: " + mapsNames[currMap]);

	if (openfile.is_open())
	{
		while (!openfile.eof())
		{
			auto x = 0;

			openfile >> x;

			TempMap.push_back(x);

			if (openfile.peek() == '\n')
			{
				Map.push_back(TempMap);
				TempMap.clear();
			}
		}
		Map.push_back(TempMap);
	}
}

void TileMap::changeMap()
{
	TempMap.clear();
	Map.clear();
	platforms.clear();

	endingMapTime = 0.0f;
	currMap++;
	loadMapFromFile();

	if (currMap == 1 || currMap == 4)
		Background.setTexture(BackgroundUnderworldTexture);
	else
		Background.setTexture(BackgroundOverworldTexture);

	mapChanged = true;
	changePlatforms = true;
}

void TileMap::initializePlatforms()
{
	if(changePlatforms)
	{
		if (currMap == 1)
		{
			for (auto i = 0; i < movingPlatformsW2.size(); i++)
			{
				platforms.emplace_back(platform);
			}
			for (auto i = 0; i < platforms.size(); i++)
			{
				platforms[i].setPosition(movingPlatformsW2[i]);
			}
		}
		else if(currMap == 3)
		{
			for (auto i = 0; i < movingPlatformsW3.size(); i++)
			{
				platforms.emplace_back(platform);
			}
			for (auto i = 0; i < platforms.size(); i++)
			{
				platforms[i].setPosition(movingPlatformsW3[i]);
			}
		}
		changePlatforms = false;
	}
}

void TileMap::removeBridge(int i, int j)
{
	if(Map[i][j] == BRIDGE_TILE_CODE)
	{
		if(bridgeTime.getElapsedTime().asSeconds() > 0.15f)
		{
			bridgeTime.restart().asSeconds();
			Map[i][j] = 0;
		}
	}
}

void TileMap::movingPlatforms(RenderWindow& W)
{
	static auto it = 0;
	
	for (auto& platform : platforms)
	{
		if(it>=440)
		{
			platformVelocity.y *= -1;
			it = 0;
		}

		platform.move(platformVelocity);
		it++;
		
		W.draw(platform);
	}
}

void TileMap::coinsDrawing(RenderWindow& W, int i, int j)
{
	coinsSprite.setPosition(static_cast<float>(j * 32), static_cast<float>(i * 32));

	if (clockCoins.getElapsedTime().asSeconds() > 0.12f)
	{
		if (RectCoins.left == 96)
			RectCoins.left = 0;
		else
			RectCoins.left += 32;

		clockCoins.restart();
	}
	coinsSprite.setTextureRect(RectCoins);

	//coinsSprites.emplace_back(make_unique<Sprite>(coinsSprite));
	coinsSprites.emplace_back(coinsSprite);
	
	W.draw(coinsSprite);
}

void TileMap::starsDrawing(RenderWindow& W, int i, int j)
{
	starSprite.setPosition(static_cast<float>(j * 32), static_cast<float>(i * 32));

	if (clockStars.getElapsedTime().asSeconds() > 0.13f)
	{
		if (RectStar.left == 192)
			RectStar.left = 128;
		else
			RectStar.left += 32;

		starSprite.setTextureRect(RectStar);
		clockStars.restart();
	}

	W.draw(starSprite);
}

void TileMap::castleFlagDrawing(RenderWindow& W, int i, int j)
{
	castleFlagSprite.setPosition(static_cast<float>(j * 32), static_cast<float>(i * 32));

	if (clockFlag.getElapsedTime().asSeconds() > 0.13f)
	{
		if (RectFlag.left >= 96)
			RectFlag.left = 0;
		else
			RectFlag.left += 32;

		castleFlagSprite.setTextureRect(RectFlag);
		clockFlag.restart();
	}
}

void TileMap::reloadAll()
{
	currMap = -1;
	changeMap();
}

void TileMap::questionMarksDrawing(RenderWindow& W, int i, int j)
{
	questionMarksSprite.setPosition(static_cast<float>(j * 32), static_cast<float>(i * 32));

	if (clockAnimTiles.getElapsedTime().asSeconds() > 0.15f)
	{
		if (RectQuestion.left == 96)
			RectQuestion.left = 0;
		else
			RectQuestion.left += 32;

		questionMarksSprite.setTextureRect(RectQuestion);
		clockAnimTiles.restart();
	}
}

void TileMap::lavaDrawing(RenderWindow& W, int i, int j)
{
	lavaSprite.setPosition(static_cast<float>(j * 32), static_cast<float>(i * 32));
	RectLava.top = 64;

	if (clockLavaTiles.getElapsedTime().asSeconds() > 0.15f)
	{
		if (RectLava.left == 96)
			RectLava.left = 0;
		else
			RectLava.left += 32;

		lavaSprite.setTextureRect(RectLava);
		clockLavaTiles.restart();
	}
}

void TileMap::princessDrawing(RenderWindow& W)
{
	if(currMap == 4)
	{
		princessSprite.setTextureRect(IntRect(222, 106, 32, 54));
		princessSprite.setPosition(5890, 428);
		W.draw(princessSprite);
	}
}
