#include "Options.h"
#include "Leaderboard.h"

Options::Options(float width, float height)
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
	menu[0].setString("Colours");
	menu[0].setPosition(Vector2f(90, 150));

	menu[1].setString("Music ON/OF");
	menu[1].setPosition(Vector2f(20, 220));

	menu[2].setString("Leaderboard");
	menu[2].setPosition(Vector2f(20, 290));

	menu[3].setString("Return");
	menu[3].setPosition(Vector2f(100, 360));

	selectedItemIndex = 0;
	ifChosen = false;
}

void Options::draw(RenderWindow& W)
{
	menu[0].setPosition(Vector2f(currXCoordinate(W) - 560, 150));
	menu[1].setPosition(Vector2f(currXCoordinate(W) - 630, 220));
	menu[2].setPosition(Vector2f(currXCoordinate(W) - 630, 290));
	menu[3].setPosition(Vector2f(currXCoordinate(W) - 550, 360));

	for (auto& v : menu)
	{
		W.draw(v);
	}
}

void Options::moveUp()
{
	if (selectedItemIndex - 1 >= 0)
	{
		menu[selectedItemIndex].setFillColor(Color(53, 44, 74));
		selectedItemIndex--;
		menu[selectedItemIndex].setFillColor(Color::Red);
	}
}

void Options::moveDown()
{
	if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
	{
		menu[selectedItemIndex].setFillColor(Color(53, 44, 74));
		selectedItemIndex++;
		menu[selectedItemIndex].setFillColor(Color::Red);
	}
}

void Options::mainMenu(Event& e, RenderWindow& W, Sound& theme, Leaderboard& menuLe)
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
					cout << "choosing colour button has been pressed" << std::endl;
					//menuL.changeIfChosen();
				}
				break;
			case 1:
				{
					cout << theme.getStatus() << std::endl;
					if (theme.getStatus() == 2)
						theme.stop();
					else
						theme.play();
				}
				break;
			case 2:
				{
					cout << "leaderboard button has been pressed" << std::endl;
					//menuLe.changeIfChosen();
				}
				break;
			case 3:
				this->changeIfChosen();
				break;
			}
			break;
		}
	}
}

bool Options::getIfChosen()
{
	return ifChosen;
}

void Options::changeIfChosen()
{
	ifChosen = !ifChosen;
}
