#pragma once
#include "GameObject.h"
#include "TileMap.h"

class Npc : public GameObject
{
	friend class Goomba;
	friend class Koopa;
	
protected:
	Vector2f velocity;

	Texture npcTexture;
	Sprite npcSprite;
	IntRect npcRect;
	
	SoundBuffer dieSoundBuffer;
	Sound dieSound;
	Clock dieTime;

	Vector2f npcPlace;
	float fNewNpcPosX{};
	float fNewNpcPosY{};
	bool faceRight{};
	bool alive;

public:
	Npc();
	Npc(const string& npcName);
	void update(float deltaTime) override;
	void draw(RenderWindow& W) override;
	void intersects(TileMap& Map, float deltaTime) override;
	~Npc() override;
};
