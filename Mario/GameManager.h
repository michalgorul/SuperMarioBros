#pragma once
#include "classes.h"
#include "Goomba.h"
#include "Leaderboard.h"
#include "OptionBase.h"
#include "Options.h"
#include "Instruction.h"
#include "TileMap.h"
#include "Mario.h"
#include "Menu.h"

class GameManager
{
private:
	
	View cameraView;
	Image icon;
	
	OptionBase* pointer;
	Menu menuM;
	Options menuO;
	Instruction menuI;
	Leaderboard menuLe;

	Texture menuTexture;
	Sprite menuSprite;
	Texture pauseTexture;
	Image endImage;
	
	
	SoundBuffer themeBuffer;
	Sound themeSound;
	SoundBuffer pauseBuffer;
	Sound pauseSound;
	
	TileMap Map;
	Mario cMario;
	Goomba mainGoomba;

	Clock clock;
	
	float deltaTime;

	ifstream leaderboard;

public:
	GameManager();
	void configure(RenderWindow& W);
	void operatingEvent(RenderWindow& W, Event& e);
	void stoppedPlaying(RenderWindow& W);
	void startedPlaying(RenderWindow& W);
	void themeSoundManager();
	void cameraMove(RenderWindow& W);
	void restartDeltaTime();
	void godFunction();

	bool start;

};
