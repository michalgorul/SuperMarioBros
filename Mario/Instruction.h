#pragma once
#include "Options.h"

class Instruction : public Options
{
private:
	bool ifChosen;

	Text fire;
	Text move;
	Text pause;

	Texture texture[MAX_NUMBER_OF_ITEMS];
	Sprite sprite[MAX_NUMBER_OF_ITEMS];
public:
	Instruction() : ifChosen(false)
	{
	};

	Instruction(float width, float height);
	void draw(RenderWindow& W) override;
	float currXCoordinate(RenderWindow& W) override { return W.getView().getCenter().x; }
	void mainMenu(Event& e, RenderWindow& W);
	void moveUp() override;
	void moveDown() override;
	bool getIfChosen();
	void changeIfChosen();
};
