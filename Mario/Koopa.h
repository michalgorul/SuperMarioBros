#pragma once
#include "Npc.h"
class Koopa : public Npc
{
private:
	IntRect hiddenRect;
public:
	Koopa(const string& npcPlace);
	Koopa();
	void update(float deltaTime) override;
	void update(float deltaTime, Mario& mario, TileMap& Map);
	void draw(RenderWindow& W) override;
	void intersects(TileMap& Map, float deltaTime) override;
	void intersects(TileMap& Map, float deltaTime, int index);
	void loadKoopasPlacesFromFile(int currMap);
	void reloadKoopas(int currMap);
	void hideKoopa(int index);
	void addToKoopasprites();
	void koopaDied(int index);
	void isTouchingWithMario(Mario& mario, float deltaTime, int index);
	void operatingKoopas(RenderWindow& W, TileMap& Map, Mario& mario, float deltaTime);
	Vector2f getKoopaPosition(int index);						// wola³bym, ¿eby w klasie mario by³a taka metoda, ¿e np. 
	vector<Vector2f> koopasPlaces;								//podaje przez argument obiekt NPC i ona sprawdza kolizje
	vector<Sprite> koopasSprites;
	vector<bool> koopaFaceRight;
	vector<bool> koopasAlive;
	vector<bool> koopaHidden;
	vector<Vector2f> velocities;
	vector<string> koopasOnMaps;
};

