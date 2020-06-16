#include "Goomba.h"


Goomba::Goomba()
{
}

Goomba::Goomba(const string& npcPlace)
{
	const ifstream openfile(npcPlace);
	if (!openfile.is_open())
		throw runtime_error("Can't load file: " + npcPlace);

	goombasOnMaps = { "goomba_1-1.txt", "goomba_1-2.txt", "goomba_1-3.txt", "goomba_1-4.txt", };

	moveSpeed = 100.0f;
	totalTime = 0.0f;
	switchTime = 0.25f;

	faceRight = false;
	npcRect = IntRect(0, 0, 32, 32);

	npcTexture.loadFromFile("Enemies.png");
	npcTexture.setRepeated(true);

	dieSoundBuffer.loadFromFile("kick.wav");
	dieSound.setBuffer(dieSoundBuffer);

	loadGoombasPlacesFromFile(0);
	addToGoombaSprites();
}

void Goomba::update(float deltaTime)
{
	totalTime += deltaTime;

	for (auto i = 0; i < goombaSprites.size(); ++i)
	{
		if (this->totalTime >= this->switchTime)
		{
			totalTime = 0;

			if (npcRect.left >= 32)
				npcRect.left = 0;
			else
				npcRect.left += 32;
		}

		if (goombaFaceRight[i])
			velocities[i].x = moveSpeed;
		else
			velocities[i].x = -moveSpeed;

		velocities[i].y += 981.0f * deltaTime;
	}
}

void Goomba::update(float deltaTime, Mario& mario, TileMap& Map)
{
	totalTime += deltaTime;

	if (mario.getIfRestarted() || Map.mapChanged)
		reloadGoombas(Map.currMap);

	for (auto i = 0; i < goombaSprites.size(); ++i)
	{	
		if (abs(mario.getPosition().x - goombaSprites[i].getPosition().x) < 650)
		{			
			isTouchingWithMario(mario, deltaTime, i);
			intersects(Map, deltaTime, i);

			if (this->totalTime >= this->switchTime)
			{
				totalTime = 0;

				if (npcRect.left >= 32)
					npcRect.left = 0;
				else
					npcRect.left += 32;
			}

			if (goombaFaceRight[i])
				velocities[i].x = moveSpeed;
			else
				velocities[i].x = -moveSpeed;

			velocities[i].y += 981.0f * deltaTime;
		}
	}
}

void Goomba::draw(RenderWindow& W)
{
	for (const auto& goombaSprite : goombaSprites)
		W.draw(goombaSprite);
}

void Goomba::intersects(TileMap& Map, float deltaTime)
{
}

void Goomba::intersects(TileMap& Map, float deltaTime, int index)
{

	const auto fNpcPosY = goombaSprites[index].getPosition().y;
	fNewNpcPosX = goombaSprites[index].getPosition().x + velocities[index].x * deltaTime;
	fNewNpcPosY = goombaSprites[index].getPosition().y + velocities[index].y * deltaTime;

	if (fNewNpcPosX >= 0 && fNewNpcPosX <= Map.nLevelWidth && fNewNpcPosY >= 0 && fNewNpcPosY + 64 <= Map.
		nLevelHeight && goombasAlive[index])
	{
		if (velocities[index].x < 0) //moving left
		{
			if ((Map.getTile(fNewNpcPosX + 0.0f, fNpcPosY + 0.0f) > 0 && Map.getTile(
				fNewNpcPosX + 0.0f, fNpcPosY + 0.0f) < 40) ||
				(Map.getTile(fNewNpcPosX + 0.0f, fNpcPosY + 0.9f) > 0 && Map.getTile(
					fNewNpcPosX + 0.0f, fNpcPosY + 0.9f) < 40) ||
				(Map.getTile(fNewNpcPosX + 0.0f, fNpcPosY + 32.0f) > 0 && Map.getTile(
					fNewNpcPosX + 0.0f, fNpcPosY + 32.0f) < 40) ||
				(Map.getTile(fNewNpcPosX + 0.0f, fNpcPosY + 32.9f) > 0 && Map.getTile(
					fNewNpcPosX + 0.0f, fNpcPosY + 32.9f) < 40))
			{
				goombaFaceRight[index] = true;
			}
		}

		else //moving right
		{
			if ((Map.getTile(fNewNpcPosX + 33.0f, fNpcPosY + 0.0f) > 0 && Map.getTile(
				fNewNpcPosX + 33.0f, fNpcPosY + 0.0f) < 40) ||
				(Map.getTile(fNewNpcPosX + 33.0f, fNpcPosY + 0.9f) > 0 && Map.getTile(
					fNewNpcPosX + 33.0f, fNpcPosY + 0.9f) < 40) ||
				(Map.getTile(fNewNpcPosX + 33.0f, fNpcPosY + 32.0f) > 0 && Map.getTile(
					fNewNpcPosX + 33.0f, fNpcPosY + 32.0f) < 40) ||
				(Map.getTile(fNewNpcPosX + 33.0f, fNpcPosY + 32.9f) > 0 && Map.getTile(
					fNewNpcPosX + 33.0f, fNpcPosY + 32.9f) < 40))
			{
				goombaFaceRight[index] = false;
			}
		}

		if (velocities[index].y >= 0) // Moving Down
		{
			if ((Map.getTile(fNewNpcPosX + 0.0f, fNewNpcPosY + 33.0f) > 0 && Map.getTile(
				fNewNpcPosX + 0.0f, fNewNpcPosY + 33.0f) < 40) ||
				(Map.getTile(fNewNpcPosX + 0.9f, fNewNpcPosY + 33.0f) > 0 && Map.getTile(
					fNewNpcPosX + 0.9f, fNewNpcPosY + 33.0f) < 40) ||
				(Map.getTile(fNewNpcPosX + 32.0f, fNewNpcPosY + 33.0f) > 0 && Map.getTile(
					fNewNpcPosX + 32.0f, fNewNpcPosY + 33.0f) < 40) ||
				(Map.getTile(fNewNpcPosX + 32.9f, fNewNpcPosY + 33.0f) > 0 && Map.getTile(
					fNewNpcPosX + 32.9f, fNewNpcPosY + 33.0f) < 40))
			{
				fNewNpcPosY = fNpcPosY;
				velocities[index].y = 0;
			}
		}
		goombaSprites[index].setTextureRect(npcRect);
		goombaSprites[index].setPosition(fNewNpcPosX, fNewNpcPosY); // Apply new position
	}
	else
	{
		goombaDied(index);
	}
	
}

void Goomba::loadGoombasPlacesFromFile(int currMap)
{
	ifstream openfile(goombasOnMaps[currMap]);
	if (!openfile.is_open())
		throw runtime_error("Can't load file: " + goombasOnMaps[currMap]);

	while (!openfile.eof())
	{
		float x = 0;
		float y = 0;

		openfile >> x;
		openfile >> y;

		goombaPlaces.emplace_back(Vector2f(x, y));
	}
}

void Goomba::reloadGoombas(const int currMap)
{
	goombaSprites.clear();
	goombasAlive.clear();
	goombaPlaces.clear();
	goombaFaceRight.clear();
	velocities.clear();
	
	if(currMap>2)
		loadGoombasPlacesFromFile(currMap - 1);
	else
		loadGoombasPlacesFromFile(currMap);

	addToGoombaSprites();
}

void Goomba::addToGoombaSprites()
{
	for (auto v = goombaPlaces.begin(); v < goombaPlaces.end(); ++v)
	{
		goombaSprites.emplace_back(npcSprite);
	}

	for (auto v = 0; v < goombaSprites.size(); ++v)
	{
		goombaSprites[v].setTexture(npcTexture);
		goombaSprites[v].setTextureRect(npcRect);
		goombaSprites[v].setPosition(goombaPlaces[v]);

		goombaFaceRight.emplace_back(false);
		velocities.emplace_back(Vector2f(0.0f, 0.0f));
		goombasAlive.emplace_back((true));
	}
}

void Goomba::goombaDied(const int index)
{
	if (goombasAlive[index])
	{
		goombasAlive[index] = false;
		dieTime.restart().asSeconds();
		velocities[index] = Vector2f(0.0f, 0.0f);
		goombaSprites[index].setTextureRect(IntRect(64, 0, 32, 32));
		goombaSprites[index].setPosition(getGoombaPosition(index));

	}
	
	if(dieTime.getElapsedTime().asSeconds()>0.2f && goombasAlive[index] == false)
	{
		eraseFromUnorderedByIndex(goombaSprites, index);
		eraseFromUnorderedByIndex(goombaFaceRight, index);
		eraseFromUnorderedByIndex(velocities, index);
		eraseFromUnorderedByIndex(goombasAlive, index);
	}
}

void Goomba::isTouchingWithMario(Mario& mario, float deltaTime, int index)
{
	if (goombaSprites[index].getGlobalBounds().intersects(mario.marioSprite.getGlobalBounds()) && mario.getIfAlive() && goombasAlive[index])
	{
		if(!mario.invincible)
		{
			if (goombaSprites[index].getGlobalBounds().top <= mario.marioSprite.getGlobalBounds().top + mario.marioSprite.getGlobalBounds().height - 25)
			{
				mario.reciveDamage(deltaTime);
			}
			else
			{
				mario.marioKilledNpc(deltaTime);
				goombaDied(index);
				dieSound.play();

			}
		}
		else
		{
			mario.marioKilledNpc(deltaTime);
			goombaDied(index);
			dieSound.play();
		}
		
	}
}

void Goomba::operatingGoombas(RenderWindow& W, TileMap& Map, Mario& mario, float deltaTime)
{
	update(deltaTime, mario, Map);
	draw(W);
}

Vector2f Goomba::getGoombaPosition(const int index)
{
	return goombaSprites[index].getPosition();
}

template <typename TContainer> //nie wiem czemu kod tej funkcji musi by równie¿ w tym pliku
void eraseFromUnorderedByIndex(TContainer& inContainer, size_t inIndex) //bez tego pojawia siê b³¹d linkera
{
	if (inIndex < inContainer.size()) //u¿ywam template do ³atwego usuwania elementów z wektora
	{
		if (inIndex != inContainer.size() - 1)
			inContainer[inIndex] = inContainer.back();
		inContainer.pop_back();
	}
}