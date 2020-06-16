#pragma once
#include "classes.h"
#include "GameObject.h"
#include "TileMap.h"

class Mario : public GameObject
{
	friend class TileMap;
	friend class Goomba;
	friend class Koopa;
	friend class Browser;
private:
	Font statsFont;
	Clock changeSpriteTime;
	Clock endGameTime;
	Clock endTime;
	Clock collectClock;
	Clock invincibleTimer;
	SoundBuffer jumpSoundBuffer;
	SoundBuffer dieSoundBuffer;
	SoundBuffer clearWorldBuffer;
	SoundBuffer invincibleBuffer;
	SoundBuffer pipeBuffer;
	SoundBuffer gameOverBuffer;
	Sound jumpSound;
	Sound dieSound;
	Sound clearWorldSound;
	Sound invincibleSound;
	Sound pipeSound;	
	Sound gameOverSound;	
	Texture marioTexture;
	Sprite marioSprite;
	IntRect rectMario = IntRect(0, 96, 32, 32);
	Vector2f velocity;
	vector<string> worlds = {"1-1", "1-2", "1-2", "1-3", "1-4" };
	vector<Vector2f> worldsRestartPositions = { Vector2f(66, 442), Vector2f(96, 64), Vector2f(56, 383), Vector2f(96, 442), Vector2f(66, 240) };
	bool faceRight;
	bool inTheAir;
	bool canJump;
	bool alive;
	bool seated;
	bool promoted;
	bool restarted;
	bool onPlatform{};
	bool levelComplete;
	bool inPipe;
	bool invincible;
	bool canCompleteLevel;
	bool gameComplete;
	float jumpHeight;
	float fNewPlayerPosX;
	float fNewPlayerPosY;
	float timeLeft;
	const float invincibleTime = 12.0f;
	int whichPlatform{};
	int currWorld;
	unsigned int coinsCounter;
	unsigned int score;
	unsigned int lives;


public:
	Mario();
	Mario(const string& marioName);
	~Mario();
	Vector2f getPosition() const;
	Vector2f getNextPosition() const;
	IntRect getSize() const { return rectMario; }
	int howManyLives();
	void update(float deltaTime) override;
	void draw(RenderWindow& W) override;
	void setPosition(float x, float y);
	void onTheFloor();
	void intersects(TileMap& Map, float deltaTime) override;
	void intersectsWithPlatforms(TileMap& Map, float deltaTime);
	void reciveDamage(float deltaTime);
	void marioDied(float deltaTime);
	void marioKilledNpc(float deltaTime);
	void marioRestart();
	void showStats(RenderWindow& W);
	void levelCompleted(TileMap& Map, float deltaTime);
	bool getIfAlive() const { return alive; }
	bool getIfRestarted() const { return restarted; }
	bool getIfInvincible();
	bool getIfLevelCompleted() const { return levelComplete; }
	bool getIfInPipe() const { return inPipe; }
	bool getIfGameCompleted() const { return gameComplete; }
	void increaseScore(int value);
	void timeToScore();
	void interactionBlockAction(TileMap& Map, float x, float y);
	void checkForCollectables(TileMap& Map);
	void promoting();
	void demoting();
	void standUp();
	void seatDown();
	void jump();
	void invincibleMode(float deltaTime);
	void starCollected();
	void coinCollected();
	void pipeOperation(TileMap& Map);
	void changeCurrWorld(int world);
	void reloadAll();
	void gameCompleted(float deltaTime);
	void finalScene(RenderWindow& W);
	bool backToMenu;
	Clock gameOverTime;;
};




