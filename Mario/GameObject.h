#pragma once
#include "classes.h"
class TileMap;

class GameObject
{
	friend class Mario;
protected:
	float moveSpeed{};
	float totalTime{};
	float switchTime{};
public:
	virtual ~GameObject();
	virtual void update(float deltaTime) = 0;
	virtual void draw(RenderWindow& W) = 0;
	virtual void intersects(TileMap& Map, float deltaTime) = 0;
};
