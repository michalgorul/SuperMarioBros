#include "Leaderboard.h"

Leaderboard::Leaderboard()
{
}

Leaderboard::Leaderboard(const float width, float height)
{
	if (!font.loadFromFile("main_font.ttf"))
		cout << "cant load file" << endl;

	float xfont = 100;

	for (auto i = 0; i < 3; i++)
	{
		menu[i].setFont(font);
		menu[i].setCharacterSize(48);
		menu[i].setPosition(Vector2f(xfont, 210));
		menu[i].setFillColor(Color::White);

		xfont += 430;
	}

	menu[0].setString("Level One");
	menu[1].setString("Level Two");
	menu[2].setString("Level Three");

	menu[3].setFont(font);
	menu[3].setCharacterSize(48);
	menu[3].setPosition(Vector2f((width / 2) - 95, 600));
	menu[3].setFillColor(Color::Red);
	menu[3].setString("Return");

	ifChosen = false;
	level1[4] = {0};
	level2[4] = {0};
	level3[4] = {0};
}

void Leaderboard::draw(RenderWindow& W)
{
	//tekstHead(W, "LEADERBOARD", 410, 40, 70);

	for (const auto& i : menu)
	{
		W.draw(i);
	}
}

void Leaderboard::mainMenu(Event& e, RenderWindow& W)
{
	if (e.type == Event::KeyReleased && e.key.code == Keyboard::Return)
		//jeœli wybra³em leaderboard - zmieniam atrybut ifChosen
		this->changeIfChosen();
}

bool Leaderboard::getIfChosen()
{
	return ifChosen;
}

void Leaderboard::changeIfChosen()
{
	ifChosen = !ifChosen;
}

void Leaderboard::readingFromFile(ifstream& leaderboard)
{
	if (leaderboard.good())
	{
		while (leaderboard >> this->level1[0])
		{
			leaderboard >> this->level1[1] >> this->level1[2] >> this->level1[3] >> this->level1[4];
			leaderboard >> this->level2[0] >> this->level2[1] >> this->level2[2] >> this->level2[3] >> this->level2[4];
			leaderboard >> this->level3[0] >> this->level3[1] >> this->level3[2] >> this->level3[3] >> this->level3[4];
		}
	}
	else
		cout << "That file doesn't exist" << endl;
}

void Leaderboard::savingToFile()
{
	ofstream save("Leaderboard.txt"); //otwieram plik do zapisu

	for (auto i : this->level1)
		save << i << endl;

	for (auto i : this->level2)
		save << i << endl;

	for (auto i : this->level3)
		save << i << endl;

	save.close(); //zamykam plik do zapisu
}

void Leaderboard::saveToLeaderboard(float time, int current_level)
{
	if (current_level == 1)
	{
		for (auto& i : level1)
		{
			if (time < i)
			{
				for (int j = 3; j >= 0; j--)
					level1[j + 1] = level1[j];

				i = time;
				break;
			}
		}
	}
	else if (current_level == 2)
	{
		for (auto& i : level2)
		{
			if (time < i)
			{
				for (auto j = 3; j >= 0; j--)
					level2[j + 1] = level2[j];

				i = time;
				break;
			}
		}
	}
	else
	{
		for (auto& i : level3)
		{
			if (time < i)
			{
				for (int j = 3; j >= 0; j--)
					level3[j + 1] = level3[j];

				i = time;
				break;
			}
		}
	}
}
