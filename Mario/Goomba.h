#pragma once
#include "Npc.h"
#include "TileMap.h"
#include "Mario.h"

class Goomba : public Npc
{
public:
	Goomba(const string& npcPlace); //obiekt z tego konstruktora bêdzie mia³ vektor spritów enpeców
	Goomba();
	void update(float deltaTime) override;
	void update(float deltaTime, Mario& mario, TileMap& Map);
	void draw(RenderWindow& W) override;
	void intersects(TileMap& Map, float deltaTime) override;
	void intersects(TileMap& Map, float deltaTime, int index);
	void loadGoombasPlacesFromFile(int currMap);
	void reloadGoombas(int currMap);
	void addToGoombaSprites();
	void goombaDied(int index);
	void isTouchingWithMario(Mario& mario, float deltaTime, int index);
	void operatingGoombas(RenderWindow& W, TileMap& Map, Mario& mario, float deltaTime);
	Vector2f getGoombaPosition(int index);						// wola³bym, ¿eby w klasie mario by³a taka metoda, ¿e np. 
	vector<Vector2f> goombaPlaces;								//podaje przez argument obiekt NPC i ona sprawdza kolizje
	vector<Sprite> goombaSprites;
	vector<bool> goombaFaceRight;
	vector<bool> goombasAlive;
	vector<Vector2f> velocities;
	vector<string> goombasOnMaps;
};


