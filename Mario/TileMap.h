#pragma once
#include "classes.h"
#include "GameObject.h"
#include "Mario.h"

constexpr auto BRICK_TILE_CODE = 2;
constexpr auto MONEY_TILE_CODE = 3;
constexpr auto COIN_TILE_CODE = 11;
constexpr auto MUSHROOM_TILE_CODE = 12;
constexpr auto STAR_BRICK_CODE = 13;
constexpr auto LADDER_TILE_CODE = 17;
constexpr auto BRIDGE_TILE_CODE = 18;
constexpr auto CHAIN_TILE_CODE = 81;
constexpr auto INIZ_UP_TILE_CODE = 20;
constexpr auto INIZ_COIN_CODE = 19;
constexpr auto LIVE_UP_CODE = 21;
constexpr auto BRICK_LADDER_TILE_CODE = 26;
constexpr auto INVIZ_LADDER_TILE_CODE = 27;
constexpr auto BRICK_MUSHROOM_TILE_CODE = 28;
constexpr auto LAVA_CODE = 82;
constexpr auto USED_QUESTION_MARK = 9;
constexpr auto CASTLE_FLAG_CODE = 90;
constexpr auto AXE_CODE = 99;
constexpr auto COIN_CODE = 100;
constexpr auto MUSHROOM_CODE = 101;
constexpr auto INIZ_UP_CODE = 102;
constexpr auto STAR_CODE = 103;

class TileMap
{
	friend class Mario;
	friend class Goomba;
	friend class Koopa;
	friend class Browser;
protected:

	vector<string> mapsNames;
	int currMap;
	
	Clock clockAnimTiles;
	Clock clockLavaTiles;
	Clock clockCoins;
	Clock clockStars;
	Clock clockFlag;
	Clock bridgeTime;
	Texture TileTexture;
	Sprite Tiles;
	Texture AnimTexture;
	Sprite questionMarksSprite;
	Sprite lavaSprite;
	Texture itemsTexture;
	Sprite coinsSprite;

	//vector<unique_ptr<Sprite>> coinsSprites;
	//vector<unique_ptr<Sprite>> mushroomSprites;

	vector<Sprite> coinsSprites;
	vector<Sprite> mushroomSprites;

	
	Sprite starSprite;
	Sprite mushroomSprite;
	Sprite inizSprite;
	Sprite castleFlagSprite;
	Sprite axeSprite;
	Sprite princessSprite;
	Texture BackgroundOverworldTexture;
	Texture BackgroundUnderworldTexture;
	Sprite Background;
	IntRect RectTiles = IntRect(0, 0, 32, 32);
	IntRect RectQuestion = IntRect(0, 0, 32, 32);
	IntRect RectLava = IntRect(0, 96, 32, 32);
	IntRect RectCoins = IntRect(0, 84, 32, 32);
	IntRect RectStar = IntRect(128, 212, 32, 32);
	IntRect RectFlag = IntRect(0, 148, 32, 32);
	
	vector<vector<int>> Map;
	vector<int> TempMap;
	Vector2f position;
	float nLevelWidth{};
	float nLevelHeight{};
	float endingMapTime;

	SoundBuffer coinBuffer;
	Sound coinSound;
	SoundBuffer powerupAppearsBuffer;
	Sound powerupAppearsSound;
	SoundBuffer powerupBuffer;
	Sound powerupSound;
	SoundBuffer blockBreakBuffer;
	Sound blockBreakSound;

	Sprite platform;
	Vector2f platformVelocity = Vector2f(0, 2);
	IntRect platformRect = IntRect(0, 0, 96, 16);
	vector<Vector2f> movingPlatformsW2;
	vector<Vector2f> movingPlatformsW3;
	vector<Sprite> platforms;
	bool changePlatforms;
	
public:
	TileMap();
	TileMap(string tilesName, string animName);
	void update(RenderWindow& W, Mario& CMario, float deltaTime);
	void draw(RenderWindow& W);
	int getTile(float x, float y);
	void setTile(float x, float y, int tileCode);
	void loadMapFromFile();
	void changeMap();//returnes map number
	int whatMap() const { return currMap; }//returnes map number
	void initializePlatforms();
	void removeBridge(int i, int j);
	void movingPlatforms(RenderWindow& W);
	void coinsDrawing(RenderWindow& W, int i, int j);
	void starsDrawing(RenderWindow& W, int i, int j);
	void questionMarksDrawing(RenderWindow& W, int i, int j);
	void lavaDrawing(RenderWindow& W, int i, int j);
	void princessDrawing(RenderWindow& W);
	void castleFlagDrawing(RenderWindow& W, int i, int j);
	void reloadAll();
	bool mapChanged;
};



