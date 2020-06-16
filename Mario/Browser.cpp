#include "Browser.h"

Browser::Browser()
{
}

Browser::Browser(string browserFile)
{
	const ifstream openfile(browserFile);
	if (!openfile.is_open())
		throw runtime_error("Can't load file: " + browserFile);

	moveSpeed = 30.0f;
	totalTime = 0.0f;
	switchTime = 0.25f;

	faceRight = false;
	browserAlive = true;
	changedDirection = false;
	canThrowFireBalls = true;
	npcTexture.loadFromFile(browserFile);
	npcTexture.setRepeated(true);
	npcSprite.setTexture(npcTexture);
	npcSprite.setTextureRect(npcRect);
	npcSprite.setPosition(startingPosition);

	browserFallsBuffer.loadFromFile("bowser_falls.wav");
	browserFireBuffer.loadFromFile("bowser_fire.wav");
	browserFallsSound.setBuffer(browserFallsBuffer);
	browserFireSound.setBuffer(browserFireBuffer);

	fireBallMoveSpeed = -100.0f;
	fireBallVelocity = Vector2f(fireBallMoveSpeed, 0);
	fireBallRect = IntRect(0, 364, 32, 26);
}

void Browser::update(float deltaTime)
{

}

void Browser::update(float deltaTime, Mario& mario, TileMap& Map)
{
	if(Map.currMap == 4)
	{
		totalTime += deltaTime;

		if (mario.getIfRestarted() || Map.mapChanged)
			reloadBrowser();

		isFireBallTouchingWithMario(mario, deltaTime);

		if (abs(mario.getPosition().x - npcSprite.getPosition().x) < 650)
		{
			isTouchingWithMario(mario, deltaTime);
			intersects(Map, deltaTime);

			if (this->totalTime >= this->switchTime)
			{
				totalTime = 0;

				if(!changedDirection)
				{
					npcRect.height = 80;
					
					if (npcRect.left >= 420)
						npcRect.left = 0;
					else
						npcRect.left += 84;
				}
				else
				{
					changedDirection = false;
				}
				
			}

			if (faceRight)
				velocity.x = moveSpeed;
			else
				velocity.x = -moveSpeed;
			
			
			
			velocity.y += 981.0f * deltaTime;
		}
		
		if (velocity.y > 5.0f && mario.getIfGameCompleted() && browserFallsSound.getStatus() != 2 && browserAlive)
			browserFallsSound.play();
		
	}
}

void Browser::draw(RenderWindow& W)
{
	W.draw(npcSprite);
}

void Browser::intersects(TileMap& Map, float deltaTime)
{
	const auto fNpcPosY = npcSprite.getPosition().y;
	fNewNpcPosX = npcSprite.getPosition().x + velocity.x * deltaTime;
	fNewNpcPosY = npcSprite.getPosition().y + velocity.y * deltaTime;

	if (fNewNpcPosX >= 0 && fNewNpcPosX <= Map.nLevelWidth && fNewNpcPosY >= 0 && fNewNpcPosY + npcRect.height <= Map.
		nLevelHeight && browserAlive)
	{
		if (velocity.x < 0) //moving left
		{
			if ((Map.getTile(fNewNpcPosX + 0.0f, fNpcPosY + 0.0f) > 0 && Map.getTile(
				fNewNpcPosX + 0.0f, fNpcPosY + 0.0f) < 40) ||
				(Map.getTile(fNewNpcPosX + 0.0f, fNpcPosY + 0.9f) > 0 && Map.getTile(
					fNewNpcPosX + 0.0f, fNpcPosY + 0.9f) < 40) ||
				(Map.getTile(fNewNpcPosX + 0.0f, fNpcPosY + npcRect.height) > 0 && Map.getTile(
					fNewNpcPosX + 0.0f, fNpcPosY + npcRect.height) < 40) ||
				(Map.getTile(fNewNpcPosX + 0.0f, fNpcPosY + npcRect.height + 0.9f) > 0 && Map.getTile(
					fNewNpcPosX + 0.0f, fNpcPosY + npcRect.height + 0.9f) < 40) || fNewNpcPosX < 4930.0f)
			{
				faceRight = true;
				changedDirection = true;
			}
		}

		else //moving right
		{
			if ((Map.getTile(fNewNpcPosX + npcRect.width + 0.9f, fNpcPosY + 0.0f) > 0 && Map.getTile(
				fNewNpcPosX + npcRect.width + 0.9f, fNpcPosY + 0.0f) < 40) ||
				(Map.getTile(fNewNpcPosX + npcRect.width + 0.9f, fNpcPosY + 0.9f) > 0 && Map.getTile(
					fNewNpcPosX + npcRect.width + 0.9f, fNpcPosY + 0.9f) < 40) ||
				(Map.getTile(fNewNpcPosX + npcRect.width + 0.9f, fNpcPosY + npcRect.height) > 0 && Map.getTile(
					fNewNpcPosX + npcRect.width + 0.9f, fNpcPosY + npcRect.height) < 40) ||
				(Map.getTile(fNewNpcPosX + npcRect.width + 0.9f, fNpcPosY + npcRect.height + 0.9f) > 0 && Map.getTile(
					fNewNpcPosX + npcRect.width + 0.9f, fNpcPosY + npcRect.height + 0.9f) < 40))
			{
				faceRight = false;
				changedDirection = true;
			}
		}

		if (velocity.y >= 0) // Moving Down
		{
			if ((Map.getTile(fNewNpcPosX + 0.0f, fNewNpcPosY + npcRect.height + 0.9f) > 0 && Map.getTile(
				fNewNpcPosX + 0.0f, fNewNpcPosY + npcRect.height + 0.9f) < 40) ||
				(Map.getTile(fNewNpcPosX + 0.9f, fNewNpcPosY + npcRect.height + 0.9f) > 0 && Map.getTile(
					fNewNpcPosX + 0.9f, fNewNpcPosY + npcRect.height + 0.9f) < 40) ||
				(Map.getTile(fNewNpcPosX + npcRect.width, fNewNpcPosY + npcRect.height + 0.9f) > 0 && Map.getTile(
					fNewNpcPosX + npcRect.width, fNewNpcPosY + npcRect.height + 0.9f) < 40) ||
				(Map.getTile(fNewNpcPosX + npcRect.width + 0.9f, fNewNpcPosY + npcRect.height + 0.9f) > 0 && Map.getTile(
					fNewNpcPosX + npcRect.width + 0.9f, fNewNpcPosY + npcRect.height + 0.9f) < 40))
			{
				fNewNpcPosY = fNpcPosY;
				velocity.y = 0;
			}
		}
		if(!faceRight)
			npcSprite.setTextureRect(npcRect);
		else
			npcSprite.setTextureRect(IntRect(npcRect.left+npcRect.width,npcRect.top,-npcRect.width,npcRect.height));

		npcSprite.setPosition(fNewNpcPosX, fNewNpcPosY); // Apply new position
	}
	else
	{
		browserDied();
	}

}

void Browser::reloadBrowser()
{
	npcSprite.setPosition(startingPosition);
	fireBalls.clear();
}

void Browser::browserDied()
{
	browserAlive = false;
	canThrowFireBalls = false;
	npcRect = IntRect(0, 0, 0, 0);
	npcSprite.setTextureRect(npcRect);
	npcSprite.setPosition(0, 0);
}

void Browser::isTouchingWithMario(Mario& mario, float deltaTime)
{
	if (npcSprite.getGlobalBounds().intersects(mario.marioSprite.getGlobalBounds()) && mario.getIfAlive())
	{

		if (!mario.invincible)
		{
			if (npcSprite.getGlobalBounds().top <= mario.marioSprite.getGlobalBounds().top + mario.marioSprite.getGlobalBounds().height - 25)
			{
				mario.reciveDamage(deltaTime);
			}
			else
			{
				mario.marioKilledNpc(deltaTime);

			}
		}
		else
		{
			mario.marioKilledNpc(deltaTime);
		}
	}
}

void Browser::isFireBallTouchingWithMario(Mario& mario, float deltaTime)
{
	for (auto i = 0; i < fireBalls.size(); ++i)
	{
		if (fireBalls[i]->getGlobalBounds().intersects(mario.marioSprite.getGlobalBounds()) && mario.getIfAlive())
		{
			if (!mario.invincible)
			{

				mario.reciveDamage(deltaTime);

			}
			else
			{
				mario.marioKilledNpc(deltaTime);
			}
		}
	}
}

void Browser::operatingBrowser(RenderWindow& W, TileMap& Map, Mario& mario, float deltaTime)
{
	if(Map.currMap == 4)
	{
		update(deltaTime, mario, Map);
		draw(W);
		FireBallOperations(W, deltaTime);
	}	
}

void Browser::FireBallOperations(RenderWindow& W, float deltaTime)
{
	if(fireBallTime.getElapsedTime().asSeconds() >= 3.5f && canThrowFireBalls)
	{
		fireBalls.emplace_back(make_unique<Sprite>(npcTexture));
		browserFireSound.play();
		fireBallTime.restart().asSeconds();
		fireBalls.back()->setTextureRect(fireBallRect);
		fireBalls.back()->setPosition(Vector2f(npcSprite.getPosition().x, npcSprite.getPosition().y + giveRandomNumber(0.0f,npcRect.height)));
	}

	for (auto i = 0; i < fireBalls.size(); ++i)
	{
		if (fireBallTimeChange.getElapsedTime().asSeconds() >= 0.2f)
		{
			fireBallTimeChange.restart().asSeconds();

			if (fireBallRect.left >= 96)
				fireBallRect.left = 0;
			else
				fireBallRect.left += 32;
		}

		fireBalls[i]->setTextureRect(IntRect(fireBallRect.left + fireBallRect.width, fireBallRect.top,-fireBallRect.width,fireBallRect.height));
		fireBalls[i]->move(fireBallVelocity*deltaTime); // Apply new position

		W.draw(*fireBalls[i]);
	}
}
