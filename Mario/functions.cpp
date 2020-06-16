#include "functions.h"


void stoppedPlaying(RenderWindow& W, Leaderboard& menuLe, Options& menuO, OptionBase* pointer, Instruction& menuI,
                    Menu& menuM, Sprite& sprite_m)
{
	Texture menuTexture;
	menuTexture.loadFromFile("menu.jpg");
	sprite_m.setTexture(menuTexture);
	const auto xCoordinate = W.getView().getCenter().x - 650.0f;
	//takie wartoœci, ¿eby Sprite by³ w sta³ym miejscu wzglêdem View
	sprite_m.setPosition(xCoordinate, 0.0f);
	W.draw(sprite_m);

	if (menuO.getIfChosen())
	{
		if (menuLe.getIfChosen())
		{
			pointer = &menuLe; //stosuje polimorfizm
			drawOption(pointer, W);
		}
		else if (menuO.getIfChosen())
		{
			pointer = &menuO; //stosuje polimorfizm
			drawOption(pointer, W);
		}
	}
	else
	{
		if (menuI.getIfChosen())
		{
			pointer = &menuI; //stosuje polimorfizm
			drawOption(pointer, W);
		}
		else
		{
			pointer = &menuM; //stosuje polimorfizm
			drawOption(pointer, W);
		}
	}
}

void startedPlaying(RenderWindow& W, Mario& cMario, TileMap& Map, Goomba& mainGoomba, Koopa& mainKoopa,
	Browser& mainBrowser, float deltaTime, Sound& theme, SoundBuffer& theme_, SoundBuffer& themeU_,
	SoundBuffer& themeB_)
{
	cMario.update(deltaTime);
	Map.update(W, cMario, deltaTime);
	cMario.intersects(Map, deltaTime);
	soundManager(cMario, theme, Map, theme_, themeU_, themeB_);

	/////////////////////////////////////////////////////////////

	mainGoomba.operatingGoombas(W, Map, cMario, deltaTime);
	mainKoopa.operatingKoopas(W, Map, cMario, deltaTime);
	mainBrowser.operatingBrowser(W, Map, cMario, deltaTime);

	/////////////////////////////////////////////////////////////////
	cMario.draw(W);
	cameraMove(W, cMario);

	if (Map.mapChanged == true)
		Map.mapChanged = false;

	if (cMario.howManyLives() <= 0)
		reloadAll(cMario, Map);
}

void operatingEvent(RenderWindow& W, Event& e, Leaderboard& menuLe, Options& menuO, Instruction& menuI, Menu& menuM,
                    Sprite& sprite_m, bool& START, Sound& theme, Sound& pause)
{
	if (e.type == Event::Closed)
	{
		Image end;
		end.loadFromFile("menu_begin.jpg");
		end.saveToFile("menu.jpg");
		W.close();
	}

	if (Keyboard::isKeyPressed(Keyboard::Escape))
	{
		START = false;
		theme.pause();
		if(pause.getStatus()!=2)
			pause.play();
				
		Texture texture;
		texture.create(W.getSize().x, W.getSize().y);
		texture.update(W);
		texture.copyToImage().saveToFile("menu.jpg");
	}

	if (menuO.getIfChosen())
	{
		if (menuI.getIfChosen())
		{
			if (menuLe.getIfChosen())
				menuLe.mainMenu(e, W);
			else
				menuI.mainMenu(e, W);
		}
		else
			menuO.mainMenu(e, W, theme, menuLe);
	}
	else
		menuM.mainMenu(e, W, START, menuO, menuI);
}

void startPlaying(RenderWindow& W, TileMap& Map)
{
	W.display();
}

void cameraMove(RenderWindow& W, Mario& cMario)
{
	View view;
	view.reset(FloatRect(0, 0, 1300, 650));
	view.setViewport(FloatRect(0, 0, 1.0f, 1.0f));
	Vector2f position(650, 325);

	if (cMario.getPosition().x + 10 > 650)
		position.x = cMario.getPosition().x + 10;
	else
		position.x = 650;

	view.setCenter(position);

	W.setView(view);
}

void soundManager(Mario& cMario, Sound& theme, TileMap& Map, SoundBuffer& theme_, SoundBuffer& themeU_, SoundBuffer& themeB_)
{
	
	if (!cMario.getIfAlive() || cMario.getIfLevelCompleted() || cMario.getIfInPipe() || cMario.getIfGameCompleted())
	{
		theme.stop();
	}

	if (cMario.getIfInvincible())
	{
		theme.setVolume(0);
	}


	if (cMario.getIfInvincible() == false)
	{
		theme.setVolume(50);
	}

	if (cMario.getIfRestarted() || theme.getStatus() == 1)
	{
		theme.play();
	}

	if(Map.mapChanged)
	{
		if (Map.whatMap() == 1)
		{
			theme.setBuffer(themeU_);	
		}
		else if(Map.whatMap() == 4)
		{
			theme.setBuffer(themeB_);			
		}
		else
		{
			theme.setBuffer(theme_);			
		}

		theme.setLoop(true);
		theme.setVolume(50);
		theme.play();
	}
}

void reloadAll(Mario& cMario, TileMap& Map)
{
	if (cMario.gameOverTime.getElapsedTime().asSeconds() > 6.7f)
	{
		cMario.reloadAll();
		Map.reloadAll();
	}
}

