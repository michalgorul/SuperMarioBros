#include "classes.h"
#include "Menu.h"
#include "Options.h"
#include "Leaderboard.h"
#include "OptionBase.h"
#include "Instruction.h"
#include "functions.h"
#include "Mario.h"
#include "Goomba.h"
#include  "Koopa.h"
#include "GameManager.h"
#include "Browser.h"

int main()
{
	RenderWindow W;
	Event e;

	auto START = false;

	Menu menuM(WINDOW_SIZE_X, WINDOW_SIZE_Y);
	Options menuO(WINDOW_SIZE_X, WINDOW_SIZE_Y);
	Instruction menuI(WINDOW_SIZE_X, WINDOW_SIZE_Y);
	Leaderboard menuLe(WINDOW_SIZE_X, WINDOW_SIZE_Y);

	TileMap Map("Tiles.png", "AnimTiles.png");

	OptionBase* pointer = nullptr;

	SoundBuffer themeOverworldBuffer;
	SoundBuffer themeUnderworldBuffer;
	SoundBuffer browserCastleBuffer;
	Sound themeSound;
	SoundBuffer pauseBuffer;
	Sound pauseSound;	

	Texture texture_m;
	Sprite sprite_m;

	//ifstream leaderboard;
	//leaderboard.open("leaderboard.txt", ios::in);
	//menuLe.readingFromFile(leaderboard);
	//leaderboard.close();

	configure(W, themeOverworldBuffer,themeUnderworldBuffer, browserCastleBuffer,themeSound, texture_m, sprite_m,pauseBuffer,pauseSound);
	themeSound.play();
	
	Mario cMario("Mario.png");
	Clock clock;
	auto deltaTime = 0.0f;

	Goomba mainGoomba("goomba_1-1.txt");
	Koopa mainKoopa("koopa_1-1.txt");
	Browser mainBrowser("Bowser.png");
	//=================================================G£ÓWNA PÊTLA============================

	//gameManager.godFunction();
	
	while (W.isOpen())
	{
		deltaTime = clock.restart().asSeconds();
		//gameManager.restartDeltaTime();

		while (W.pollEvent(e))
		{
			//gameManager.operatingEvent(W, e);
			operatingEvent(W, e, menuLe, menuO, menuI, menuM, sprite_m, START, themeSound, pauseSound);
		}

		W.clear(Color::Black);

		if (START == false)
		{
			//gameManager.stoppedPlaying(W);
			stoppedPlaying(W, menuLe, menuO, pointer, menuI, menuM, sprite_m);
		}

		if (START == true)
		{
			//gameManager.startedPlaying(W);
			startedPlaying(W, cMario, Map, mainGoomba, mainKoopa, mainBrowser, deltaTime, themeSound, themeOverworldBuffer,
			               themeUnderworldBuffer, browserCastleBuffer);

			if (cMario.backToMenu == true)
			{
				START = false;
				reloadAll(cMario, Map);
			}
		}
		W.display();
	}

	return 0;
}
