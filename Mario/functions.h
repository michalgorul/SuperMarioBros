#pragma once
#include "Leaderboard.h"
#include "Options.h"
#include "OptionBase.h"
#include "Instruction.h"
#include "TileMap.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Browser.h"

void stoppedPlaying(RenderWindow& W, Leaderboard& menuLe, Options& menuO, OptionBase* pointer, Instruction& menuI,
                    Menu& menuM, Sprite& sprite_m);

void startedPlaying(RenderWindow& W, Mario& cMario, TileMap& Map, Goomba& mainGoomba, Koopa& mainKoopa, Browser& mainBrowser, float deltaTime, Sound& theme, SoundBuffer& theme_, SoundBuffer& themeU_, SoundBuffer& themeB_);

void operatingEvent(RenderWindow& W, Event& e, Leaderboard& menuLe, Options& menuO, Instruction& menuI, Menu& menuM,
                    Sprite& sprite_m, bool& START, Sound& theme, Sound& pause);

void startPlaying(RenderWindow& W, TileMap& Map);

void cameraMove(RenderWindow& W, Mario& cMario);

void soundManager(Mario& cMario, Sound& theme, TileMap& Map, SoundBuffer& theme_, SoundBuffer& themeU_, SoundBuffer& themeB_);

void reloadAll(Mario& cMario, TileMap& Map);

