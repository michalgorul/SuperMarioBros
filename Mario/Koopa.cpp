#include "Koopa.h"

Koopa::Koopa(const string& npcPlace)
{
	const ifstream openfile(npcPlace);
	if (!openfile.is_open())
		throw runtime_error("Can't load file: " + npcPlace);

	koopasOnMaps = { "koopa_1-1.txt", "koopa_1-2.txt", "koopa_1-3.txt", "koopa_1-4.txt" };

	hiddenRect = IntRect(192,32,32,45);
	moveSpeed = 80.0f;
	totalTime = 0.0f;
	switchTime = 0.25f;

	faceRight = false;
	npcRect = IntRect(0, 32, 32, 45);

	npcTexture.loadFromFile("Enemies.png");
	npcTexture.setRepeated(true);

	dieSoundBuffer.loadFromFile("kick.wav");
	dieSound.setBuffer(dieSoundBuffer);

	loadKoopasPlacesFromFile(0);
	addToKoopasprites();
}

Koopa::Koopa()
{
}

void Koopa::update(float deltaTime)
{
}

void Koopa::update(float deltaTime, Mario& mario, TileMap& Map)
{
	totalTime += deltaTime;

	if (mario.getIfRestarted() || Map.mapChanged)
		reloadKoopas(Map.currMap);

	for (auto i = 0; i < koopasSprites.size(); ++i)
	{
		if (abs(mario.getPosition().x - koopasSprites[i].getPosition().x) < 750)
		{
			intersects(Map, deltaTime, i);
			isTouchingWithMario(mario, deltaTime, i);
			
			if (this->totalTime >= this->switchTime)
			{
				totalTime = 0;

				if (npcRect.left >= 32)
					npcRect.left = 0;
				else
					npcRect.left += 32;
			}

			if (koopaFaceRight[i])
				velocities[i].x = moveSpeed;
			else
				velocities[i].x = -moveSpeed;

			velocities[i].y += 981.0f * deltaTime;
		}
		if (koopaHidden[i])
		{
			velocities[i].x = 0.0f;
			koopasSprites[i].setTextureRect(hiddenRect);
		}

	}
}

void Koopa::draw(RenderWindow& W)
{
	for (const auto& koopaSprite : koopasSprites)
		W.draw(koopaSprite);
}

void Koopa::intersects(TileMap& Map, float deltaTime)
{
}

void Koopa::intersects(TileMap& Map, float deltaTime, int index)
{
	for (auto v = 0; v < koopasSprites.size(); ++v)
	{
		const auto fNpcPosY = koopasSprites[index].getPosition().y;
		fNewNpcPosX = koopasSprites[index].getPosition().x + velocities[index].x * deltaTime;
		fNewNpcPosY = koopasSprites[index].getPosition().y + velocities[index].y * deltaTime;

		if (fNewNpcPosX >= 0 && fNewNpcPosX <= Map.nLevelWidth && fNewNpcPosY >= 0 && fNewNpcPosY + 64 <= Map.
			nLevelHeight && koopasAlive[index])
		{
			if (velocities[index].x < 0) //moving left
			{
				if ((Map.getTile(fNewNpcPosX + 0.0f, fNpcPosY + 0.0f) > 0 && Map.getTile(
					fNewNpcPosX + 0.0f, fNpcPosY + 0.0f) < 40) ||
					(Map.getTile(fNewNpcPosX + 0.0f, fNpcPosY + 0.9f) > 0 && Map.getTile(
						fNewNpcPosX + 0.0f, fNpcPosY + 0.9f) < 40) ||
					(Map.getTile(fNewNpcPosX + 0.0f, fNpcPosY + npcRect.height) > 0 && Map.getTile(
						fNewNpcPosX + 0.0f, fNpcPosY + npcRect.height) < 40) ||
					(Map.getTile(fNewNpcPosX + 0.0f, fNpcPosY + npcRect.height + 0.9f) > 0 && Map.getTile(
						fNewNpcPosX + 0.0f, fNpcPosY + npcRect.height + 0.9f) < 40))
				{
					koopaFaceRight[index] = true;
				}
			}

			else //moving right
			{
				if ((Map.getTile(fNewNpcPosX + 33.0f, fNpcPosY + 0.0f) > 0 && Map.getTile(
					fNewNpcPosX + 33.0f, fNpcPosY + 0.0f) < 40) ||
					(Map.getTile(fNewNpcPosX + 33.0f, fNpcPosY + 0.9f) > 0 && Map.getTile(
						fNewNpcPosX + 33.0f, fNpcPosY + 0.9f) < 40) ||
					(Map.getTile(fNewNpcPosX + 33.0f, fNpcPosY + npcRect.height) > 0 && Map.getTile(
						fNewNpcPosX + 33.0f, fNpcPosY + npcRect.height) < 40) ||
					(Map.getTile(fNewNpcPosX + 33.0f, fNpcPosY + npcRect.height + 0.9f) > 0 && Map.getTile(
						fNewNpcPosX + 33.0f, fNpcPosY + npcRect.height + 0.9f) < 40))
				{
					koopaFaceRight[index] = false;
				}
			}

			if (velocities[index].y >= 0) // Moving Down
			{
				if ((Map.getTile(fNewNpcPosX + 0.0f, fNewNpcPosY + npcRect.height + 1.0f) > 0 && Map.getTile(
					fNewNpcPosX + 0.0f, fNewNpcPosY + npcRect.height + 1.0f) < 40) ||
					(Map.getTile(fNewNpcPosX + 0.9f, fNewNpcPosY + npcRect.height + 1.0f) > 0 && Map.getTile(
						fNewNpcPosX + 0.9f, fNewNpcPosY + npcRect.height + 1.0f) < 40) ||
					(Map.getTile(fNewNpcPosX + 32.0f, fNewNpcPosY + 33.0f) > 0 && Map.getTile(
						fNewNpcPosX + 32.0f, fNewNpcPosY + npcRect.height + 1.0f) < 40) ||
					(Map.getTile(fNewNpcPosX + 32.9f, fNewNpcPosY + npcRect.height + 1.0f) > 0 && Map.getTile(
						fNewNpcPosX + 32.9f, fNewNpcPosY + npcRect.height + 1.0f) < 40))
				{
					fNewNpcPosY = fNpcPosY;
					velocities[index].y = 0;
				}
			}
			
			if (koopaHidden[index] == true)
				koopasSprites[index].setTextureRect(hiddenRect);
			else
			{
				if (!koopaFaceRight[index])
					koopasSprites[index].setTextureRect(npcRect);
				else
					koopasSprites[index].setTextureRect(IntRect(npcRect.left + npcRect.width, npcRect.top, -npcRect.width, npcRect.height));
			}
			
			koopasSprites[index].setPosition(fNewNpcPosX, fNewNpcPosY); // Apply new position
		}
		else
		{
			koopaDied(index);
		}
	}
}

void Koopa::loadKoopasPlacesFromFile(const int currMap)
{
	ifstream openfile(koopasOnMaps[currMap]);
	if (!openfile.is_open())
		throw runtime_error("Can't load file: " + koopasOnMaps[currMap]);

	while (!openfile.eof())
	{
		float x = 0;
		float y = 0;

		openfile >> x;
		openfile >> y;

		koopasPlaces.emplace_back(Vector2f(x, y));
	}
}

void Koopa::reloadKoopas(const int currMap)
{
	koopasSprites.clear();
	koopasAlive.clear();
	koopasPlaces.clear();
	koopaFaceRight.clear();
	velocities.clear();
	koopaHidden.clear();
	
	if(currMap>2)
		loadKoopasPlacesFromFile(currMap - 1);
	else
		loadKoopasPlacesFromFile(currMap);

	addToKoopasprites();
}

void Koopa::hideKoopa(int index)
{
	koopaHidden[index] = true;
}

void Koopa::addToKoopasprites()
{
	for (auto v = koopasPlaces.begin(); v < koopasPlaces.end(); ++v)
	{
		koopasSprites.emplace_back(npcSprite);
	}

	for (auto v = 0; v < koopasSprites.size(); ++v)
	{
		koopasSprites[v].setTexture(npcTexture);
		koopasSprites[v].setTextureRect(npcRect);
		koopasSprites[v].setPosition(koopasPlaces[v]);

		koopaFaceRight.emplace_back(false);
		velocities.emplace_back(Vector2f(0.0f, 0.0f));
		koopasAlive.emplace_back(true);
		koopaHidden.emplace_back(false);
	}
}

void Koopa::koopaDied(int index)
{
	if (koopasAlive[index])
	{
		koopasAlive[index] = false;
		dieTime.restart().asSeconds();
		velocities[index] = Vector2f(0.0f, 0.0f);
		koopasSprites[index].setTextureRect(IntRect(64, 32, 32, 44));
		koopasSprites[index].setPosition(getKoopaPosition(index));
	}

	if (dieTime.getElapsedTime().asSeconds() > 0.2f && koopasAlive[index] == false)
	{
		eraseFromUnorderedByIndex(koopasSprites, index);
		eraseFromUnorderedByIndex(koopaFaceRight, index);
		eraseFromUnorderedByIndex(velocities, index);
		eraseFromUnorderedByIndex(koopasAlive, index);
		eraseFromUnorderedByIndex(koopaHidden, index);
	}
}

void Koopa::isTouchingWithMario(Mario& mario, float deltaTime, int index)
{	
	if (koopasSprites[index].getGlobalBounds().intersects(mario.marioSprite.getGlobalBounds()) && mario.getIfAlive() && koopasAlive[index])
	{
		if (!mario.invincible && mario.alive )
		{
			
			if (koopasSprites[index].getGlobalBounds().top <= mario.marioSprite.getGlobalBounds().top + mario.marioSprite.getGlobalBounds().height - 25)
			{
				mario.reciveDamage(deltaTime);
			}
			else
			{
				if (koopaHidden[index])
				{
					mario.marioKilledNpc(deltaTime);
					koopaDied(index);
					dieSound.play();
				}
				else
				{
					mario.marioKilledNpc(deltaTime);
					hideKoopa(index);
					dieSound.play();
				}
			}
		}
		else
		{
			if(koopaHidden[index] )
			{
				mario.marioKilledNpc(deltaTime);
				koopaDied(index);
				dieSound.play();
			}
			else
			{
				hideKoopa(index);
				mario.marioKilledNpc(deltaTime);
			}
		}
	}
	
}

void Koopa::operatingKoopas(RenderWindow& W, TileMap& Map, Mario& mario, float deltaTime)
{
	update(deltaTime, mario, Map);
	draw(W);
}

Vector2f Koopa::getKoopaPosition(const int index)
{
	return koopasSprites[index].getPosition();
}