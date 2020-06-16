#pragma once
#include "Npc.h"

class Browser : public Npc
{
private:
	IntRect npcRect = IntRect(0,0,84,80);
	Vector2f startingPosition = Vector2f(5200,265);
	SoundBuffer browserFireBuffer;
	Sound browserFireSound;
	SoundBuffer browserFallsBuffer;
	Sound browserFallsSound;
	bool browserAlive;
	bool changedDirection;
	bool canThrowFireBalls;
	vector<unique_ptr<Sprite>> fireBalls;
	Vector2f fireBallVelocity;
	float fireBallMoveSpeed;
	IntRect fireBallRect;
	Clock fireBallTime;
	Clock fireBallTimeChange;
	
	
public:
	Browser();
	Browser(string browserFile);
	void update(float deltaTime) override;
	void update(float deltaTime, Mario& mario, TileMap& Map);
	void draw(RenderWindow& W) override;
	void intersects(TileMap& Map, float deltaTime) override;
	void reloadBrowser();
	void browserDied();
	void isTouchingWithMario(Mario& mario, float deltaTime);
	void isFireBallTouchingWithMario(Mario& mario, float deltaTime);
	void operatingBrowser(RenderWindow& W, TileMap& Map, Mario& mario, float deltaTime);
	void FireBallOperations(RenderWindow& W, float deltaTime);
};



