#include "Mario.h"

Mario::Mario()
{
}

Mario::Mario(const string& marioName): faceRight(true), inTheAir(false), canJump(true), alive(true), seated(false),
                                       promoted(false), restarted(false), levelComplete(false), inPipe(false),
                                       invincible(false), fNewPlayerPosX(0.0f), fNewPlayerPosY(0.0f), timeLeft(180.0f),
                                       whichPlatform(0), currWorld(0), coinsCounter(0), score(0), canCompleteLevel(true),
									   backToMenu(false), lives(3)
{
	const ifstream openfile(marioName);
	if (!openfile.is_open())
		throw runtime_error("Can't load file: " + marioName);

	marioTexture.loadFromFile(marioName);
	marioTexture.setRepeated(true);
	marioSprite.setTexture(marioTexture);
	marioSprite.setPosition(worldsRestartPositions[0]);

	jumpSoundBuffer.loadFromFile("jump.wav");
	dieSoundBuffer.loadFromFile("mario_die.wav");
	clearWorldBuffer.loadFromFile("world_clear.wav");
	pipeBuffer.loadFromFile("pipe.wav");
	gameOverBuffer.loadFromFile("game_over.wav");
	jumpSound.setBuffer(jumpSoundBuffer);
	dieSound.setBuffer(dieSoundBuffer);
	clearWorldSound.setBuffer(clearWorldBuffer);
	pipeSound.setBuffer(pipeBuffer);
	gameOverSound.setBuffer(gameOverBuffer);
	
	invincibleBuffer.loadFromFile("invincibility.ogg");
	invincibleSound.setBuffer(invincibleBuffer);
	invincibleSound.setVolume(50);
	
	jumpHeight = 138.0f;
	moveSpeed = 40.0f;
	totalTime = 0.0f;
	switchTime = 0.2f;

	statsFont.loadFromFile("some_font.ttf");
}

Mario::~Mario()
{
}

int Mario::howManyLives()
{
	return lives;
}

void Mario::update(float deltaTime)
{
	velocity.x *= 0.8f;
	totalTime += deltaTime;

	if (alive && !levelComplete && !inPipe && !gameComplete)
	{
		if ((!Keyboard::isKeyPressed(Keyboard::Right) && !Keyboard::isKeyPressed(Keyboard::Left) &&
			!Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::A)) && !seated)
		{
			rectMario.left = 0;
		}

		else if (velocity.x != 0 && !seated)
		{
			if (!inTheAir)
				if (totalTime >= switchTime)
				{
					totalTime = 0;
					if (rectMario.left >= 96)
						rectMario.left = 0;
					rectMario.left += 32;
				}
		}

		if ((Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) && !seated)
		{
			faceRight = true;
			velocity.x += moveSpeed;
		}

		if ((Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) && !seated)
		{
			faceRight = false;
			velocity.x -= moveSpeed;
		}

		if ((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) && canJump)
		{
			jump();
		}

		if (!Keyboard::isKeyPressed(Keyboard::Up) && !Keyboard::isKeyPressed(Keyboard::W))
		{
			if (velocity.y < 0)	//! jeœli nie przytrzymamy skoku mario nie skoczy na pe³n¹ mo¿liw¹ wysokoœæ
				velocity.y *= 0.8;
		}

		if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S) && promoted)
		{
			seatDown();
		}
		else if (!Keyboard::isKeyPressed(Keyboard::Down) || !Keyboard::isKeyPressed(Keyboard::S) && promoted)
		{
			standUp();
		}

		if(inTheAir)
			rectMario.left = 160;

		if (faceRight)
			marioSprite.setTextureRect(rectMario);
		else
			marioSprite.setTextureRect(IntRect(rectMario.left + rectMario.width, rectMario.top, -rectMario.width, rectMario.height));

		if (invincible)
		{
			invincibleMode(deltaTime);			// jeœli nieœmiertelnoœæ
		}
		else
			invincibleTimer.restart().asSeconds();

		if (restarted)
			restarted = false;
		
		endGameTime.restart().asSeconds();
		endTime.restart().asSeconds();
		gameOverTime.restart().asSeconds();
	}
	else if (gameComplete)
		gameCompleted(deltaTime);
	
	if(!onPlatform)
		velocity.y += 981.0f * deltaTime;
}

void Mario::draw(RenderWindow& W)
{
	W.draw(marioSprite);
	showStats(W);

	if (gameComplete)
		finalScene(W);
}

Vector2f Mario::getPosition() const
{
	return marioSprite.getPosition();
}

Vector2f Mario::getNextPosition() const
{
	return {fNewPlayerPosX, fNewPlayerPosY};
}

void Mario::setPosition(float x, float y)
{
	marioSprite.setPosition(x, y);
}

void Mario::onTheFloor()
{
	velocity.y = 0;
	canJump = true;
	inTheAir = false;
}

void Mario::intersects(TileMap& Map, float deltaTime)
{
	const auto fPlayerPosX = marioSprite.getPosition().x;
	const auto fPlayerPosY = marioSprite.getPosition().y;
	fNewPlayerPosX = marioSprite.getPosition().x + velocity.x * deltaTime;
	fNewPlayerPosY = marioSprite.getPosition().y + velocity.y * deltaTime;

	if(levelComplete == false)
	{
		checkForCollectables(Map);
		intersectsWithPlatforms(Map, deltaTime);
		pipeOperation(Map);

		if (fNewPlayerPosX >= 0 && fNewPlayerPosX <= Map.nLevelWidth && fNewPlayerPosY + rectMario.height + 32.0f <= Map.
			nLevelHeight && alive)
		{
			
			if (velocity.x < 0) //moving left
			{
				if ((Map.getTile(fNewPlayerPosX, fPlayerPosY) > 0 && Map.getTile(fNewPlayerPosX, fPlayerPosY) < 40) ||
					(Map.getTile(fNewPlayerPosX, fPlayerPosY + 0.9f) > 0 && Map.getTile(fNewPlayerPosX, fPlayerPosY + 0.9f) < 40) ||
					(Map.getTile(fNewPlayerPosX, fPlayerPosY + rectMario.height) > 0 && Map.getTile(fNewPlayerPosX, fPlayerPosY + rectMario.height) < 40) ||
					(Map.getTile(fNewPlayerPosX, fPlayerPosY + rectMario.height + 0.9f) > 0 && Map.getTile(fNewPlayerPosX, fPlayerPosY + rectMario.height + 0.9f) < 40))
				{
					fNewPlayerPosX = fPlayerPosX;
					velocity.x = 0;
				}
			}

			else //moving right
			{
				if ((Map.getTile(fNewPlayerPosX + rectMario.width + 1.0f, fPlayerPosY) > 0 && Map.getTile(fNewPlayerPosX + rectMario.width + 1.0f, fPlayerPosY + 0.0f) < 40) ||
					(Map.getTile(fNewPlayerPosX + rectMario.width + 1.0f, fPlayerPosY + 0.9f) > 0 && Map.getTile(fNewPlayerPosX + rectMario.width + 1.0f, fPlayerPosY + 0.9f) < 40) ||
					(Map.getTile(fNewPlayerPosX + rectMario.width + 1.0f, fPlayerPosY + rectMario.height) > 0 && Map.getTile(fNewPlayerPosX + rectMario.width + 1.0f, fPlayerPosY + rectMario.height) < 40) ||
					(Map.getTile(fNewPlayerPosX + rectMario.width + 1.0f, fPlayerPosY + rectMario.height + 0.9f) > 0 && Map.getTile(fNewPlayerPosX + rectMario.width + 1.0f, fPlayerPosY + rectMario.height + 0.9f) < 40))
				{
					fNewPlayerPosX = marioSprite.getPosition().x;
					velocity.x = 0;
				}
			}

			if (velocity.y <= 0) // Moving Up
			{
				if ((Map.getTile(fNewPlayerPosX, fNewPlayerPosY) > 0 && Map.getTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY) < 40) ||
					(Map.getTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY) > 0 && Map.getTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY) < 40) ||
					(Map.getTile(fNewPlayerPosX + rectMario.width, fNewPlayerPosY) > 0 && Map.getTile(fNewPlayerPosX + rectMario.width, fNewPlayerPosY) < 40) ||
					(Map.getTile(fNewPlayerPosX + rectMario.width + 0.9f, fNewPlayerPosY) > 0 && Map.getTile(fNewPlayerPosX + rectMario.width + 0.9f, fNewPlayerPosY) < 40))
				{
					fNewPlayerPosY = fPlayerPosY;
					velocity.y = 0;
					interactionBlockAction(Map, marioSprite.getGlobalBounds().left, marioSprite.getGlobalBounds().top);
				}
			}

			else // Moving Down
			{
				canJump = false;

				if ((Map.getTile(fNewPlayerPosX, fNewPlayerPosY + rectMario.height + 1.0f) > 0 && Map.getTile(fNewPlayerPosX, fNewPlayerPosY + rectMario.height + 1.0f) < 40) ||
					(Map.getTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY + rectMario.height + 1.0f) > 0 && Map.getTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY + rectMario.height + 1.0f) < 40) ||
					(Map.getTile(fNewPlayerPosX + rectMario.width, fNewPlayerPosY + rectMario.height + 1.0f) > 0 && Map.getTile(fNewPlayerPosX + rectMario.width, fNewPlayerPosY + rectMario.height + 1.0f) < 40) ||
					(Map.getTile(fNewPlayerPosX + rectMario.width + 0.9f, fNewPlayerPosY + rectMario.height + 1.0f) > 0 && Map.getTile(fNewPlayerPosX + rectMario.width + 0.9f, fNewPlayerPosY + rectMario.height + 1.0f) < 40))
				{
					fNewPlayerPosY = fPlayerPosY;

					velocity.y = 0;
					onTheFloor(); // Player has a solid surface underfoot
				}
			}
			
			if (Map.getTile(fNewPlayerPosX, fNewPlayerPosY) == 80 || Map.getTile(fNewPlayerPosX, fNewPlayerPosY) == 72)
				levelComplete = true;
			
			else if (onPlatform)
			{
				if (Map.platformVelocity.y < 0)
					marioSprite.setPosition(fNewPlayerPosX, Map.platforms[0].getPosition().y - rectMario.height - 2.0f);
				else
					marioSprite.setPosition(fNewPlayerPosX, Map.platforms[0].getPosition().y - rectMario.height);

			}
			
			else
				marioSprite.setPosition(fNewPlayerPosX, fNewPlayerPosY); // Apply new position

			if (Map.currMap == 2 && marioSprite.getPosition().x > 2000)
				marioRestart();
			
		}
		else
		{
			marioDied(deltaTime);
		}
	}
	else
		levelCompleted(Map, deltaTime);
}

void Mario::intersectsWithPlatforms(TileMap& Map, float deltaTime)
{
	for(auto i = 0; i < Map.platforms.size(); i++)
	{
		if (Map.platforms[i].getGlobalBounds().intersects(marioSprite.getGlobalBounds()))
		{
			onTheFloor();
			onPlatform = true;
			whichPlatform = i;
		}
		
		else if ((marioSprite.getPosition().x >= Map.platforms[whichPlatform].getPosition().x + 96.0f ||
			marioSprite.getPosition().x + 32.0f <= Map.platforms[whichPlatform].getPosition().x) && onPlatform)
		{
			onPlatform = false;
			marioSprite.setPosition(marioSprite.getPosition().x, marioSprite.getPosition().y - 2.0f);
		}
	}
}

void Mario::reciveDamage(float deltaTime)
{
	if (!invincible)
	{
		if (promoted)
			demoting();
		else
			marioDied(deltaTime);
	}
}

void Mario::marioDied(float deltaTime)
{
	if (lives > 0)
	{
		static Clock dieTime;

		alive = false;

		static auto wasExecuted = false;
		if (!wasExecuted)
		{
			velocity.y = -sqrtf(4.0f * 981.0f * jumpHeight);
			dieTime.restart().asSeconds();
			dieSound.play();
		}

		wasExecuted = true;
		velocity.y += 220.0f * deltaTime;
		rectMario.left = 192;
		rectMario.top = 96;
		marioSprite.setTextureRect(rectMario);
		marioSprite.setPosition(fNewPlayerPosX, fNewPlayerPosY);


		if (dieTime.getElapsedTime().asSeconds() > 2.95)
		{
			lives--;
			if (lives > 0)
				marioRestart();

			wasExecuted = false;
		}
	}
	else if(gameOverSound.getStatus() != 2)
		gameOverSound.play();
	
}	

void Mario::marioKilledNpc(float deltaTime)
{
	velocity.y = -sqrtf(0.5f * 981.0f * jumpHeight);
	increaseScore(10);
}

void Mario::marioRestart()
{
	marioSprite.setPosition(worldsRestartPositions[currWorld]);
	rectMario.left = 0;
	rectMario.top = 96;
	rectMario.height = 32;
	faceRight = true;
	canJump = true;
	alive = true;
	timeLeft = 180.0f;
	promoted = false;
	inPipe = false;
	canCompleteLevel = true;

	restarted = true;
}

void Mario::showStats(RenderWindow& W)
{
	static Text scoreText;
	static Text timeLeftText;
	static Text livesText;
	static Text worldNameText;

	static Clock timeInStats;

	auto xCoordinate = W.getView().getCenter().x - 615.0f;
	//takie wartoœci, ¿eby tekst by³ w sta³ym miejscu wzglêdem View
	const auto yCoordinate = 10.0f;

	vector<Text> texts;

	texts.push_back(scoreText);
	texts.push_back(livesText);
	texts.push_back(worldNameText);
	texts.push_back(timeLeftText);

	if (timeInStats.getElapsedTime().asSeconds() >= 1.0f && timeLeft > 0)
	{
		timeLeft--;
		timeInStats.restart();
	}

	scoreText.setString("SCORE: " + to_string(score));
	livesText.setString("LIVES: " + to_string(lives));
	worldNameText.setString("WORLD: " + worlds[currWorld]);
	timeLeftText.setString("TIME: " + to_string(static_cast<int>(timeLeft)));


	for (auto& v : texts)
	{
		v.setFont(statsFont);
		v.setFillColor(Color(53, 44, 74));
		v.setCharacterSize(48);
		v.setPosition(xCoordinate, yCoordinate);
		xCoordinate += 330;

		W.draw(v);
	}
}

void Mario::levelCompleted(TileMap& Map, float deltaTime)
{
	totalTime += deltaTime;
	
	if (timeLeft <= 0)
		Map.endingMapTime += deltaTime;
	
	if(canCompleteLevel)
	{
		rectMario.left = 224;
		marioSprite.setPosition(marioSprite.getPosition().x - 8.0f, marioSprite.getPosition().y);
		clearWorldSound.play();
		canCompleteLevel = false;
	}
	
	timeToScore();	//! dodajemy punkty za pozosta³y czas
	
	if(Map.getTile(marioSprite.getPosition().x, marioSprite.getPosition().y + rectMario.height - 32.0f) == 80 ||
		Map.getTile(marioSprite.getPosition().x, marioSprite.getPosition().y + rectMario.height - 32.0f) == 72 ||
		Map.getTile(marioSprite.getPosition().x + rectMario.width, marioSprite.getPosition().y + rectMario.height) == 80 ||
		Map.getTile(marioSprite.getPosition().x + rectMario.width, marioSprite.getPosition().y + rectMario.height) == 72)
	{
		if (totalTime >= switchTime + 0.4f)
		{
			totalTime = 0;

			if(!promoted)
			{
				if (rectMario.left >= 256)
					rectMario.left = 224;
				else
					rectMario.left += 32;
			}
			else
			{
				if (rectMario.left > 258)
					rectMario.left = 258;
				else
					rectMario.left += 32;
			}
			
			marioSprite.setTextureRect(rectMario);
		}
		velocity.y = 2.5f;
		velocity.x = 0.0f;
		marioSprite.move(velocity);
	}
	
	else if(Map.getTile(marioSprite.getPosition().x, marioSprite.getPosition().y) == 4 ||
			Map.getTile(marioSprite.getPosition().x, marioSprite.getPosition().y) == 1 ||
			Map.getTile(marioSprite.getPosition().x + rectMario.width, marioSprite.getPosition().y + rectMario.height) == 4 ||
			Map.getTile(marioSprite.getPosition().x + rectMario.width, marioSprite.getPosition().y + rectMario.height) == 1)
		{
				faceRight = true;
		
				if (totalTime >= switchTime + 0.2f)
				{
					totalTime = 0;

					if (rectMario.left >= 96)
						rectMario.left = 0;
					else
						rectMario.left += 32;

					marioSprite.setTextureRect(rectMario);
				}

				velocity.x = 4.0f;

			
				if (Map.getTile(marioSprite.getPosition().x, marioSprite.getPosition().y + rectMario.height) != 1 &&
					Map.getTile(marioSprite.getPosition().x + rectMario.width, marioSprite.getPosition().y + rectMario.height) != 4)
					velocity.y = 2.5f;
				else
					velocity.y = 0.0f;

				if (Map.getTile(marioSprite.getPosition().x, marioSprite.getPosition().y + rectMario.height - 32.0f) == 76)
				{
					rectMario.left = 450;
					marioSprite.setTextureRect(rectMario);
					velocity.x = 0.0f;
					velocity.y = 0.0f;
					
					if(Map.currMap == 0)
						Map.setTile(6720, 320, CASTLE_FLAG_CODE);
					else if (Map.currMap == 2)
						Map.setTile(896, 320, CASTLE_FLAG_CODE);
					else if (Map.currMap == 3)
						Map.setTile(5696, 128, CASTLE_FLAG_CODE);
					
				}
		
				if (Map.endingMapTime >= 2.5f)
				{
					Map.changeMap();
					changeCurrWorld(Map.currMap);
					marioRestart();
					levelComplete = false;
				}

				marioSprite.move(velocity);
		}		
}

bool Mario::getIfInvincible()
{
	return invincible;
}

void Mario::increaseScore(const int value)
{
	score += value;
}

void Mario::timeToScore()
{
	static Clock timeToDecrease;

	timeLeft = static_cast<int>(timeLeft);  

	if (timeLeft>0)
	{
		if (timeToDecrease.getElapsedTime().asSeconds() >= 0.025f)
		{
			timeLeft--;
			timeToDecrease.restart();
			increaseScore(100);
		}
	}
}

void Mario::interactionBlockAction(TileMap& Map, float x, float y)	//! zamieniam bloki, które dotkn¹³em podczas skoku g³ow¹
{
	if(y - 32.0f > 0)
	{
		//! COINS FROM QUESTION MARKS	
		if (Map.getTile(x + 0.0f, y - 32.0f) == COIN_TILE_CODE)	//! from left
		{
			Map.setTile(x + 0.0f, y - 32.0f, COIN_CODE);
			Map.setTile(x + 0.0f, y, USED_QUESTION_MARK);

		}
		else if (Map.getTile(x + rectMario.width, y - 32.0f) == COIN_TILE_CODE)//! from right
		{
			Map.setTile(x + rectMario.width, y - 32.0f, COIN_CODE);
			Map.setTile(x + rectMario.width, y, USED_QUESTION_MARK);
		}

		//! MUSHROOMS FROM QUESTION MARKS
		else if (Map.getTile(x + 0.0f, y - 32.0f) == MUSHROOM_TILE_CODE)//! from left
		{
			Map.setTile(x + 0.0f, y - 32.0f, MUSHROOM_CODE);
			Map.powerupAppearsSound.play();
			Map.setTile(x + 0.0f, y, USED_QUESTION_MARK);
		}
		else if (Map.getTile(x + rectMario.width, y - 32.0f) == MUSHROOM_TILE_CODE)//! from right
		{
			Map.setTile(x + rectMario.width, y - 32.0f, MUSHROOM_CODE);
			Map.powerupAppearsSound.play();
			Map.setTile(x + rectMario.width, y, USED_QUESTION_MARK);
		}

		//! MUSHROOMS FROM BRICKS
		else if (Map.getTile(x + 0.0f, y - 32.0f) == BRICK_MUSHROOM_TILE_CODE)//! from left
		{
			Map.setTile(x + 0.0f, y - 32.0f, MUSHROOM_CODE);
			Map.powerupAppearsSound.play();
			Map.setTile(x + 0.0f, y, BRICK_TILE_CODE);
		}
		else if (Map.getTile(x + rectMario.width, y - 32.0f) == BRICK_MUSHROOM_TILE_CODE)//! from right
		{
			Map.setTile(x + rectMario.width, y - 32.0f, MUSHROOM_CODE);
			Map.powerupAppearsSound.play();
			Map.setTile(x + rectMario.width, y, BRICK_TILE_CODE);
		}

		//! DISTROYING BLOCKS
		else if (Map.getTile(x + 0.0f, y - 32.0f) == BRICK_TILE_CODE)//! from left
		{
			Map.setTile(x + 0.0f, y, 0);
			Map.blockBreakSound.play();
		}
		else if (Map.getTile(x + rectMario.width, y - 32.0f) == BRICK_TILE_CODE)//! from right
		{
			Map.setTile(x + rectMario.width, y, 0);
			Map.blockBreakSound.play();
		}

		//! COINS FROM BRICKS
		else if (Map.getTile(x + 0.0f, y - 32.0f) == MONEY_TILE_CODE)	//! from left
		{
			Map.setTile(x + 0.0f, y - 32.0f, COIN_CODE);
			Map.setTile(x + 0.0f, y, BRICK_TILE_CODE);

		}
		else if (Map.getTile(x + rectMario.width, y - 32.0f) == MONEY_TILE_CODE)//! from right
		{
			Map.setTile(x + rectMario.width, y - 32.0f, COIN_CODE);
			Map.setTile(x + rectMario.width, y, BRICK_TILE_CODE);
		}

		//! INIZ UP BLOCKS
		else if (Map.getTile(x + 0.0f, y - 32.0f) == INIZ_UP_TILE_CODE)//! from left
		{
			Map.setTile(x + 0.0f, y - 32.0f, INIZ_UP_CODE);
			Map.powerupAppearsSound.play();
			Map.setTile(x + 0.0f, y, 0);
		}
		else if (Map.getTile(x + rectMario.width, y - 32.0f) == INIZ_UP_TILE_CODE)//! from right
		{
			Map.setTile(x + rectMario.width, y - 32.0f, INIZ_UP_CODE);
			Map.powerupAppearsSound.play();
			Map.setTile(x + rectMario.width, y, 0);
		}

		//! LIVE UP BLOCKS
		else if (Map.getTile(x + 0.0f, y - 32.0f) == LIVE_UP_CODE)//! from left
		{
			Map.setTile(x + 0.0f, y - 32.0f, INIZ_UP_CODE);
			Map.powerupAppearsSound.play();
			Map.setTile(x + 0.0f, y, 0);
		}
		else if (Map.getTile(x + rectMario.width, y - 32.0f) == LIVE_UP_CODE)//! from right
		{
			Map.setTile(x + rectMario.width, y - 32.0f, INIZ_UP_CODE);
			Map.powerupAppearsSound.play();
			Map.setTile(x + rectMario.width, y, 0);
		}

		//! STARS FROM BLOCKS
		else if (Map.getTile(x + 0.0f, y - 32.0f) == STAR_BRICK_CODE)//! from left
		{
			Map.setTile(x + 0.0f, y - 32.0f, STAR_CODE);
			Map.powerupAppearsSound.play();
			Map.setTile(x + 0.0f, y, BRICK_TILE_CODE);
		}
		else if (Map.getTile(x + rectMario.width, y - 32.0f) == STAR_BRICK_CODE)//! from right
		{
			Map.setTile(x + rectMario.width, y - 32.0f, STAR_CODE);
			Map.powerupAppearsSound.play();
			Map.setTile(x + rectMario.width, y, BRICK_TILE_CODE);
		}
	}
	
}

void Mario::checkForCollectables(TileMap& Map)//! zamieniam bloki na które naszed³ na pusty blok 
{
	//? na razie dzia³a w miare, chcia³bym ¿eby te collectable by³y w wektorze spritów

	//for(auto i = 0; i < Map.coinsSprites.size(); i++)
	//{
	//	//! COINS FROM QUESTION MARKS
	//	if (Map.coinsSprites[i] && marioSprite.getGlobalBounds().intersects(Map.coinsSprites[i]->getGlobalBounds()))
	//	{
	//		Map.setTile(Map.coinsSprites[i]->getPosition().x, Map.coinsSprites[i]->getPosition().y + 32.0f, 0);
	//		Map.coinsSprites[i].reset();

	//		if(collectClock.getElapsedTime().asSeconds()>0.2f)
	//		{
	//			collectClock.restart().asSeconds();
	//			increaseScore(100);
	//			Map.coinSound.play();
	//			coinCollected();
	//		}
	//	}
	//}
	if(Map.coinsSprites.size()>0)
	{
		for (auto i = 0; i < Map.coinsSprites.size(); i++)
		{
			//! COINS FROM QUESTION MARKS
			if (marioSprite.getGlobalBounds().intersects(Map.coinsSprites[i].getGlobalBounds()))
			{
				Map.setTile(Map.coinsSprites[i].getPosition().x, Map.coinsSprites[i].getPosition().y + 32.0f, 0);
				eraseFromUnorderedByIndex(Map.coinsSprites, i);
				if (collectClock.getElapsedTime().asSeconds() > 0.2f)
				{
					collectClock.restart().asSeconds();
					increaseScore(100);
					Map.coinSound.play();
					coinCollected();
				}
			}
		}
	}
	

	//for (auto i = 0; i < Map.mushroomSprites.size(); i++)
	//{
	//	//! COINS FROM QUESTION MARKS
	//	if (Map.mushroomSprites[i] && marioSprite.getGlobalBounds().intersects(Map.mushroomSprites[i]->getGlobalBounds()))
	//	{
	//		Map.setTile(Map.mushroomSprites[i]->getPosition().x, Map.mushroomSprites[i]->getPosition().y + 32.0f, 0);
	//		Map.mushroomSprites[i].reset();

	//		if (collectClock.getElapsedTime().asSeconds() > 0.2f)
	//		{
	//			collectClock.restart().asSeconds();
	//			increaseScore(1000);
	//			Map.powerupSound.play();
	//			promoting();
	//		}
	//	}
	//}
	if(Map.mushroomSprites.size()>0)
	{
		for (auto i = 0; i < Map.mushroomSprites.size(); i++)
		{
			//! COINS FROM QUESTION MARKS
			if (marioSprite.getGlobalBounds().intersects(Map.mushroomSprites[i].getGlobalBounds()))
			{
				Map.setTile(Map.mushroomSprites[i].getPosition().x, Map.mushroomSprites[i].getPosition().y + 32.0f, 0);
				eraseFromUnorderedByIndex(Map.mushroomSprites, i);
				if (collectClock.getElapsedTime().asSeconds() > 0.2f)
				{
					collectClock.restart().asSeconds();
					increaseScore(1000);
					Map.powerupSound.play();
					promoting();
				}
			}
		}
	}

	
	//! STARS
	if (marioSprite.getGlobalBounds().intersects(Map.starSprite.getGlobalBounds()))
	{
		increaseScore(2000);
		Map.setTile(Map.starSprite.getPosition().x, Map.starSprite.getPosition().y + 32.0f, 0);
		Map.starSprite.setPosition(Vector2f(0, 0));
		//? TUTAJ W£O¯YÆ CHWILOW¥ NIEŒMIERTELNOŒÆ
		starCollected();
	}
	
	//! INIZ UPS
	else if (marioSprite.getGlobalBounds().intersects(Map.inizSprite.getGlobalBounds()) && marioSprite.getPosition().x > 400)
	{
		increaseScore(1500);
		Map.powerupSound.play();	
		Map.setTile(Map.inizSprite.getPosition().x, Map.inizSprite.getPosition().y + 32.0f, 0);
		Map.inizSprite.setPosition(Vector2f(0, 0));
		//? DODATKOWE ¯YCIE
		lives++;
	}

	else if (marioSprite.getGlobalBounds().intersects(Map.axeSprite.getGlobalBounds()) && marioSprite.getPosition().x > 400)
	{

		Map.setTile(Map.axeSprite.getPosition().x, Map.axeSprite.getPosition().y + 32.0f, 0);
		Map.axeSprite.setPosition(Vector2f(0, 0));
		gameComplete = true;
		clearWorldSound.play();
		//? TUTAJ W£O¯YÆ KONIEC GRY
	}
}

void Mario::promoting()
{
	if (!promoted)
	{
		marioSprite.move(Vector2f(0.0f, 30.0f));
		velocity.y = -sqrtf(2 * 981.0f * jumpHeight);
		rectMario.top = 34;
		rectMario.height = 62;
		promoted = true;
		
	}		
}

void Mario::demoting()
{
	if (promoted)
	{
		marioSprite.move(Vector2f(0.0f, 30.0f));
		velocity.y = -sqrtf(2 * 981.0f * jumpHeight);
		rectMario.top = 96;
		rectMario.height = 32;
		promoted = false;
	}
}

void Mario::standUp()
{
	if (seated && promoted)
	{
		rectMario.height = 62;
		rectMario.top = 34;
		marioSprite.move(Vector2f(0,32 - 50));
		seated = false;
	}
}

void Mario::seatDown()
{
	if (!seated && promoted)
	{
		rectMario.left = 192;
		rectMario.top = 52;
		rectMario.height = 44;
		marioSprite.move(Vector2f(0, 50 - 32));
		seated = true;
		velocity.x *= 0.4f;
	}
}

void Mario::jump()
{
	jumpSound.play();
	canJump = false;
	inTheAir = true;
	velocity.y = -sqrtf(2.0f * 981.0f * jumpHeight);

	if (onPlatform)
		onPlatform = false;
}

void Mario::invincibleMode(float deltaTime)
{

	if (invincibleTimer.getElapsedTime().asSeconds() > invincibleTime)
	{
		invincible = false;
	}
}

void Mario::starCollected()
{
	invincibleSound.play();
	invincible = true;
}

void Mario::coinCollected()
{
	coinsCounter++;
}

void Mario::pipeOperation(TileMap& Map)
{
	if (Map.getTile(marioSprite.getPosition().x + rectMario.width + 8.0f, marioSprite.getPosition().y + rectMario.height) == 29 ||
		Map.getTile(marioSprite.getPosition().x + rectMario.width + 8.0f, marioSprite.getPosition().y) == 29 ||
		Map.getTile(marioSprite.getPosition().x + rectMario.width + 8.0f, marioSprite.getPosition().y + rectMario.height) == 37 ||
		Map.getTile(marioSprite.getPosition().x + rectMario.width + 8.0f, marioSprite.getPosition().y + rectMario.height) == 37)
	{
		inPipe = true;
		if(pipeSound.getStatus()!=2)
			pipeSound.play();
		
		faceRight = true;

		if (totalTime >= switchTime + 0.2f)
		{
			totalTime = 0;

			if (rectMario.left >= 96)
				rectMario.left = 0;
			else
				rectMario.left += 32;

			marioSprite.setTextureRect(rectMario);
		}

		velocity.x = 4.0f;
		
		marioSprite.move(Vector2f(2.0f, 0.0f));

		if (Map.getTile(marioSprite.getPosition().x + 8.0f, marioSprite.getPosition().y + rectMario.height) == 29 ||
			Map.getTile(marioSprite.getPosition().x + 8.0f, marioSprite.getPosition().y) == 29 ||
			Map.getTile(marioSprite.getPosition().x + 8.0f, marioSprite.getPosition().y + rectMario.height) == 37 ||
			Map.getTile(marioSprite.getPosition().x + 8.0f, marioSprite.getPosition().y + rectMario.height) == 37)
		{
			
			Map.changeMap();
			changeCurrWorld(Map.currMap);
			marioRestart();
		}
	}

}

void Mario::changeCurrWorld(int world)
{
	currWorld = world;
}

void Mario::reloadAll()
{
	currWorld = 0;
	score = 0;
	coinsCounter = 0;
	lives = 3;
	backToMenu = false;
	gameComplete = false;
	marioRestart();
}

void Mario::gameCompleted(float deltaTime)
{
	timeToScore();	//! dodajemy punkty za pozosta³y czas
	totalTime += deltaTime;
	faceRight = true;

	if (marioSprite.getPosition().x < 5835 && endGameTime.getElapsedTime().asSeconds() > 3.0f)
	{
		velocity.x += moveSpeed - 15.0f;
		
		if (totalTime >= switchTime + 0.1f)
		{
			totalTime = 0;
			if (rectMario.left >= 96)
				rectMario.left = 0;
			else
			rectMario.left += 32;

			marioSprite.setTextureRect(rectMario);
		}
	}
	else
	{
		velocity = Vector2f(0, 0);
		rectMario.left = 0;
		marioSprite.setTextureRect(rectMario);
	}

	if (endTime.getElapsedTime().asSeconds() > 12.0f)
		backToMenu = true;
}

void Mario::finalScene(RenderWindow& W)
{
	static Text endText;

	const auto xCoordinate = W.getView().getCenter().x - 160.0f;
	const auto yCoordinate = 200.0f;

	endText.setString("Thank you Mario! \n  But our princess is in \n   another castle!");


	endText.setFont(statsFont);
	endText.setFillColor(Color(255, 230, 230));
	endText.setCharacterSize(48);
	endText.setPosition(xCoordinate, yCoordinate);

	if(marioSprite.getPosition().x >= 5835)
		W.draw(endText);

}
