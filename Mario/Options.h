#pragma once
#include "classes.h"
#include "Menu.h"

class Options : public Menu
{
private:
	bool ifChosen;

public:
	Options() : ifChosen(false)
	{
	}
	Options(float width, float height);
	void draw(RenderWindow& W) override;
	float currXCoordinate(RenderWindow& W) override { return W.getView().getCenter().x; }
	void moveUp() override;
	void moveDown() override;
	int getPressedItem() override { return selectedItemIndex; }
	void mainMenu(Event& e, RenderWindow& W, Sound& theme, Leaderboard& menuLe);
	bool getIfChosen();
	void changeIfChosen();
};
