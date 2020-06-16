#include "Instruction.h"

Instruction::Instruction(float width, float height)
{
	if (!font.loadFromFile("main_font.ttf"))
		cout << "cant load file" << endl;

	fire.setFont(font);
	move.setFont(font);
	pause.setFont(font);


	fire.setString("Fire/Sprint: Shift/CTRL");
	move.setString("Move: Arrows/WASD");
	pause.setString("Pause: Escape");

	fire.setCharacterSize(34);
	move.setCharacterSize(34);
	pause.setCharacterSize(34);

	fire.setFillColor(Color(53, 44, 74));
	move.setFillColor(Color(53, 44, 74));
	pause.setFillColor(Color(53, 44, 74));

	fire.setPosition(Vector2f(20, 150));
	move.setPosition(Vector2f(20, 200));
	pause.setPosition(Vector2f(20, 250));

	fire.setString("Fire/Sprint: Shift/CTRL");
	move.setString("Move: Arrows/WASD");
	pause.setString("Pause: Escape");

	fire.setPosition(Vector2f(20, 150));
	move.setPosition(Vector2f(20, 200));
	pause.setPosition(Vector2f(20, 250));

	menu[0].setFont(font);
	menu[0].setFillColor(Color::Red);
	menu[0].setCharacterSize(48);
	menu[0].setString("Return");
	menu[0].setPosition(Vector2f(20, 310));

	selectedItemIndex = 0;
	ifChosen = false;
}

void Instruction::draw(RenderWindow& W)
{
	fire.setPosition(Vector2f(currXCoordinate(W) - 630, 150));
	move.setPosition(Vector2f(currXCoordinate(W) - 630, 200));
	pause.setPosition(Vector2f(currXCoordinate(W) - 630, 250));
	menu[0].setPosition(Vector2f(currXCoordinate(W) - 630, 310));

	W.draw(fire);
	W.draw(move);
	W.draw(pause);
	W.draw(menu[0]);
}

void Instruction::moveUp()
{
	if (selectedItemIndex - 1 >= 0)
	{
		menu[selectedItemIndex].setFillColor(Color::White);
		selectedItemIndex--;
		menu[selectedItemIndex].setFillColor(Color::Red);
	}
}

void Instruction::moveDown()
{
	if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
	{
		menu[selectedItemIndex].setFillColor(Color::White);
		selectedItemIndex++;
		menu[selectedItemIndex].setFillColor(Color::Red);
	}
}

void Instruction::mainMenu(Event& e, RenderWindow& W)
{
	if (e.type == Event::KeyReleased && e.key.code == Keyboard::Return)
		//jeœli wybra³em leaderboard - zmieniam atrybut ifChosen
		this->changeIfChosen();
}

bool Instruction::getIfChosen()
{
	return ifChosen;
}

void Instruction::changeIfChosen()
{
	ifChosen = !ifChosen;
}
