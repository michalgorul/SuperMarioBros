#pragma once
#include "classes.h"

//jedna klasa bazowa dla hierarchi klas
//u¿ywam klasy abstrakcyjnej
class OptionBase
{
public:
	virtual ~OptionBase() = default;
	//metody wirtualne
	virtual void draw(RenderWindow& W) = 0;
	virtual void moveUp() = 0;
	virtual void moveDown() = 0;
	virtual int getPressedItem() = 0;
	virtual float currXCoordinate(RenderWindow& W) = 0;
	
	float xCoordnate = 0;
	float yCoordnate = 0;
};

//funkcja pomocnicza
void drawOption(OptionBase* pointer, RenderWindow& W);
