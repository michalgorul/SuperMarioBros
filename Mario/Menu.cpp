#include "Menu.h"
#include "Options.h"
#include "Instruction.h"

Menu::Menu(float width, float height)
{
	if (!font.loadFromFile("main_font.ttf"))
		cout << "cant load file" << endl;

	for (auto& v : menu)
	{
		v.setFont(font);
		v.setFillColor(Color(53, 44, 74));
		v.setCharacterSize(48);
	}

	menu[0].setFillColor(Color::Red);
	menu[0].setString("Play");
	menu[0].setPosition(Vector2f(157, 150));

	menu[1].setString("Instruction");
	menu[1].setPosition(Vector2f(22, 220));

	menu[2].setString("Options");
	menu[2].setPosition(Vector2f(107, 290));

	menu[3].setString("Exit");
	menu[3].setPosition(Vector2f(157, 360));

	selectedItemIndex = 0;
}

void Menu::draw(RenderWindow& W)
{
	menu[0].setPosition(Vector2f(currXCoordinate(W) - 493, 150));
	menu[1].setPosition(Vector2f(currXCoordinate(W) - 628, 220));
	menu[2].setPosition(Vector2f(currXCoordinate(W) - 543, 290));
	menu[3].setPosition(Vector2f(currXCoordinate(W) - 493, 360));

	for (const auto& v : menu)
	{
		W.draw(v);
	}
}

void Menu::moveUp()
{
	if (selectedItemIndex - 1 >= 0)
	{
		menu[selectedItemIndex].setFillColor(Color(53, 44, 74));
		selectedItemIndex--;
		menu[selectedItemIndex].setFillColor(Color::Red);
	}
}

void Menu::moveDown()
{
	if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
	{
		menu[selectedItemIndex].setFillColor(Color(53, 44, 74));
		selectedItemIndex++;
		menu[selectedItemIndex].setFillColor(Color::Red);
	}
}

void Menu::mainMenu(Event& e, RenderWindow& W, bool& START, Options& menuO, Instruction& menuI)
{
	switch (e.type)
	{
	case Event::KeyReleased:
		switch (e.key.code)
		{
		case Keyboard::Up:
			this->moveUp();
			break;

		case Keyboard::Down:
			this->moveDown();
			break;

		case Keyboard::Return:
			switch (this->getPressedItem())
			{
			case 0:
				{
					cout << "Play button has been pressed" << std::endl;
					START = true;
				}
				break;
			case 1:
				{
					cout << "instruction button has been pressed" << std::endl;
					START = false;
					menuI.changeIfChosen();
				}
				break;
			case 2:
				{
					cout << "Option button has been pressed" << std::endl;
					START = false;
					menuO.changeIfChosen();
				}
				break;
			case 3:
				{
					Image end;
					end.loadFromFile("menu_begin.jpg");
					end.saveToFile("menu.jpg");
					W.close();
				}
				break;
			}

			break;
		}
	}
}
