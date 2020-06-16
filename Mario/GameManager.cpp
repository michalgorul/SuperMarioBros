#include "GameManager.h"

GameManager::GameManager()
{
	icon.loadFromFile("mario.ico");
	
	menuM = Menu(WINDOW_SIZE_X, WINDOW_SIZE_Y);
	menuO = Options(WINDOW_SIZE_X, WINDOW_SIZE_Y);
	menuI = Instruction(WINDOW_SIZE_X, WINDOW_SIZE_Y);
	menuLe = Leaderboard(WINDOW_SIZE_X, WINDOW_SIZE_Y);

	leaderboard.open("leaderboard.txt", ios::in);
	menuLe.readingFromFile(leaderboard);
	leaderboard.close();

	//Map = TileMap("Tiles.png", "AnimTiles.png", "background.png");

	//cMario = Mario("Mario.png");
	mainGoomba = Goomba("goomba_1-1.txt");
	mainGoomba.addToGoombaSprites();
	
	deltaTime = 0.0f;
}

void GameManager::configure(RenderWindow &W)
{
	W.create(VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y, 32), "Super Mario Bros");
	W.setActive(true);
	W.setKeyRepeatEnabled(false);
	W.setFramerateLimit(60);
	W.setPosition(Vector2i(350, 200));
	W.setIcon(256, 256, icon.getPixelsPtr());

	themeBuffer.loadFromFile("overworld.ogg");
	themeSound.setBuffer(themeBuffer);
	themeSound.setLoop(true);
	themeSound.setVolume(50);

	pauseBuffer.loadFromFile("pause.wav");
	pauseSound.setBuffer(pauseBuffer);

	menuTexture.loadFromFile("menu.jpg");
	menuSprite.setTexture(menuTexture);
}

void GameManager::operatingEvent(RenderWindow& W, Event& e)
{
	if (e.type == Event::Closed)
	{
		endImage.loadFromFile("menu_begin.jpg");
		endImage.saveToFile("menu.jpg");
		W.close();
	}

	if (Keyboard::isKeyPressed(Keyboard::Escape))
	{
		start = false;
		//! theme.pause();
		if (pauseSound.getStatus() != 2)
			pauseSound.play();
		
		pauseTexture.create(W.getSize().x, W.getSize().y);
		pauseTexture.update(W);
		pauseTexture.copyToImage().saveToFile("menu.jpg");
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
			menuO.mainMenu(e, W, themeSound, menuLe);
	}
	else
		menuM.mainMenu(e, W, start, menuO, menuI);
}

void GameManager::stoppedPlaying(RenderWindow& W)
{
	Texture menuTexture;
	menuTexture.loadFromFile("menu.jpg");
	menuSprite.setTexture(menuTexture);
	const auto xCoordinate = W.getView().getCenter().x - 650.0f;
	
	//takie wartoœci, ¿eby Sprite by³ w sta³ym miejscu wzglêdem View
	menuSprite.setPosition(xCoordinate, 0.0f);
	W.draw(menuSprite);

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

void GameManager::startedPlaying(RenderWindow& W)
{
	cMario.update(deltaTime);
	Map.update(W, cMario, deltaTime);
	cMario.intersects(Map, deltaTime);
	themeSoundManager();

	/////////////////////////////////////////////////////////////
	mainGoomba.update(deltaTime);
	mainGoomba.intersects(Map, deltaTime);
	mainGoomba.draw(W);

	/////////////////////////////////////////////////////////////////
	cMario.draw(W);
	cameraMove(W);
}

void GameManager::themeSoundManager()
{
	if (!cMario.getIfAlive())
		themeSound.stop();
	else
	{
		if (themeSound.getStatus() == 0)
			themeSound.play();
	}
}

void GameManager::cameraMove(RenderWindow& W)
{
	cameraView.reset(FloatRect(0, 0, 1300, 650));
	cameraView.setViewport(FloatRect(0, 0, 1.0f, 1.0f));
	Vector2f position(650, 325);

	if (cMario.getPosition().x + 10 > 650)
		position.x = cMario.getPosition().x + 10;
	else
		position.x = 650;

	cameraView.setCenter(position);

	W.setView(cameraView);
}

void GameManager::restartDeltaTime()
{
	deltaTime = clock.restart().asSeconds();
}

void GameManager::godFunction()
{
	RenderWindow W;
	Event e{};
	configure(W);

	
	while (W.isOpen())
	{
		//deltaTime = clock.restart().asSeconds();
		restartDeltaTime();

		while (W.pollEvent(e))
		{
			operatingEvent(W, e);
			//operatingEvent(W, e, menuLe, menuO, menuI, menuM, sprite_m, START, theme, pause);
		}

		W.clear(Color::Black);

		if (start == false)
		{
			stoppedPlaying(W);
			//stoppedPlaying(W, menuLe, menuO, pointer, menuI, menuM, sprite_m);
		}

		if (/*START == true*/start == true)
		{
			startedPlaying(W);
			//startedPlaying(W, cMario, Map, mainGoomba, deltaTime, theme);
		}
		W.display();
	}
}
