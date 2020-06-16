#pragma once
#include "classes.h"
#include "OptionBase.h"

class Menu : public OptionBase
{
	friend class Options;
	friend class Instruction;
	friend class Leaderboard;
private:
	int selectedItemIndex;
	Font font;
	Text menu[MAX_NUMBER_OF_ITEMS];
	//float xCoordinate

public:
	Menu() : selectedItemIndex(0)
	{
	}
	Menu(float width, float height);
	void draw(RenderWindow& W) override;
	float currXCoordinate(RenderWindow& W) override { return W.getView().getCenter().x; }
	void moveUp() override;
	void moveDown() override;
	int getPressedItem() override { return selectedItemIndex; }
	void mainMenu(Event& e, RenderWindow& W, bool& START, Options& menuO, Instruction& menuI);
};
