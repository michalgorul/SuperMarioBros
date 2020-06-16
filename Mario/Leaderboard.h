#pragma once
#include "Menu.h"

class Leaderboard : public Menu
{
private:
	bool ifChosen;
	float level1[5]; //atrybuty do wyœwietlenia statystyk
	float level2[5];
	float level3[5];
	
public:
	Leaderboard();
	Leaderboard(float width, float height);
	void draw(RenderWindow& W) override;
	void mainMenu(Event& e, RenderWindow& W);
	bool getIfChosen();
	void changeIfChosen();
	void saveToLeaderboard(float x, int current_level);
	void readingFromFile(ifstream& leaderboard); //odczyt z pliku
	void savingToFile(); //zapis do pliku
};
