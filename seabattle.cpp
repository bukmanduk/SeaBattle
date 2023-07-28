#include "seabattle.h"

SeaBattle::SeaBattle() : m_human(), m_bot()
{}

void SeaBattle::Start()
{
	PreStart();
	m_bot.PutsShip();
	m_human.PutsShip();
	RemakeBoardsForStart();
	while (!m_human.EmptyShip() && !m_bot.EmptyShip())
	{
		HumanMove();
		BotMove();
	}
	End();
}

void SeaBattle::RemakeBoardsForStart()
{
	m_human.SetGeneralStep(0); m_bot.SetGeneralStep(0);
	for (int point = 0; point < BOARD_LEN * BOARD_LEN; point++)
	{
		m_bot.SetSymbolOnBoard(point, GAME_SYMBOL);
		if (m_human.GetSymbolFromBoard(point) == CLOSE_SYMBOL || m_human.GetSymbolFromBoard(point) == START_SYMBOL)
			m_human.SetSymbolOnBoard(point, GAME_SYMBOL);
		else m_human.SetStepOnBoard(point, 0);
	}
	m_human.SetGeneralStep(1); m_bot.SetGeneralStep(1);
	m_human.ResetCount(); m_bot.ResetCount();
}

bool SeaBattle::CellCorrect(std::string& input)
{
	if (size(input) < 2) { Error(7); return false; }
	char check = input[0];
	if (check < '0' || check > '9') { Error(8); return false; }
	check = input[1];
	if (check < 'A' || check > 'J') if (check < 'a' || check > 'j') { Error(9); return false; }

	int point = board::StringToPoint(input);
	if (m_bot.GetSymbolFromBoard(point) != GAME_SYMBOL) { Error(6); return false; }
	return true;
}

void SeaBattle::HumanMove()
{
	std::string input; int point = 0, count = 0, err_count = 0;

	while (!m_bot.EmptyShip())
	{
		DisplayMoving(true);

		std::cout << "\n\n\n ---> Enter cell number: ";
		Console::Read(input);

		err_count = 0;
		while (!CellCorrect(input))
		{
			err_count++;
			if (err_count % 5 != 0) std::cout << "\n\n ---> Enter cell number (example: 1A): ";
			else std::cout << "\n      So boring... Bot fell asleep.\n ---> Enter cell number: ";
			Console::Read(input);
		}
		point = board::StringToPoint(input);

		if (m_bot.GetAdressFromBoard(point) != 0)
		{
			ship* obj = m_bot.GetAdressFromBoard(point); count = 0;
			obj->Hit();
			if (obj->ShipDestroyed()) for (int i = 0; i < BOARD_LEN * BOARD_LEN; i++)
			{
				if (m_bot.GetAdressFromBoard(i) == obj)
				{
					m_bot.SetSymbolOnBoard(i, SHIP_SYMBOL);
					count++; if (count == obj->GetDeck()) break;
				}
			}
			else m_bot.SetSymbolOnBoard(point, HIT);
			if (obj->ShipDestroyed()) m_bot.CountMinus(obj->GetDeck() - 1);
		}
		else
		{
			m_bot.SetSymbolOnBoard(point, MISS);
			break;
		}
	}
}

void SeaBattle::FindBadOrientations(int point, int step, std::vector<char>& orientation)
{
	if (!GoodPoint(point + step) || point + step > point / BOARD_LEN * BOARD_LEN + 9) orientation[0] = 0;
	if (!GoodPoint(point - step) || point - step < point / BOARD_LEN * BOARD_LEN) orientation[1] = 0;
	if (!GoodPoint(point - BOARD_LEN * step)) orientation[2] = 0;
	if (!GoodPoint(point + BOARD_LEN * step)) orientation[3] = 0;
}

bool SeaBattle::GoodPoint(int point)
{
	if (point < 0 || point >= BOARD_LEN * BOARD_LEN) return false;
	if (m_human.GetStepFromBoard(point) != 0) return false;
	return true;
}

void SeaBattle::BotMove()
{
	static std::vector<char> orientation = { RIGHT, LEFT, UP, DOWN };	// все направления
	static std::vector<int> boardNum; if (boardNum.empty()) for (int i = 0; i < 100; i++) boardNum.push_back(i);
	static int generalPoint = -1; static int generalStep = 0;
	static bool test = true;
	
	srand(static_cast<unsigned int>(time(0)));
	int rNumBoard = 0;										// рандомный номер точки доски
	int rNumOrientation = 0;								// рандомный номер возможных направлений

	std::vector<int>::iterator numFind;
	std::vector<char> chooseOrientation;					// выбранные направления (возможные)

	while (!m_human.EmptyShip())
	{
		DisplayMoving();
		if (generalPoint == -1)
		{
			if (test)
			{
				rNumBoard = 22; test = false;
			}
			else rNumBoard = rand() % boardNum.size();
			rNumBoard = boardNum[rNumBoard];
			generalPoint = rNumBoard;
		}

		// если попал
		if (m_human.GetAdressFromBoard(generalPoint) != 0)
		{
			ship* obj = m_human.GetAdressFromBoard(generalPoint);
			if (GoodPoint(generalPoint))	// если бьем первый раз (т.к. после заверш. функции точка запоминается)
			{ 
				obj->Hit(); 
				m_human.SetSymbolOnBoard(generalPoint, HIT);
				std::cout << "\n\n    > Bot will hit a cell: " << board::PointToString(generalPoint) << "\n ---> ";
				Console::Pause();
				DisplayMoving();
			}

			while (!obj->ShipDestroyed())
			{
				// находим возможные направления куда бить дальше
				if (generalStep == 0) FindBadOrientations(generalPoint, 1, orientation);
				// если уже подбито 2 точки - есть максимум 2 направления
				if (generalStep == 1) if (chooseOrientation.size() > 1)
					if (chooseOrientation[rNumOrientation] == RIGHT || chooseOrientation[rNumOrientation] == LEFT)
					{
						orientation[2] = 0; orientation[3] = 0;
					}
					else
					{
						orientation[0] = 0; orientation[1] = 0;
					}
				// проверка: если 2 (3/4) точка если под ней (над/рядом) плохая точка
				if (generalStep > 0) if (chooseOrientation.size() > 1)
				{
					if (chooseOrientation[rNumOrientation] == RIGHT)
						if (!GoodPoint(generalPoint + generalStep) || generalPoint + generalStep > generalPoint / BOARD_LEN * BOARD_LEN + 9)
							orientation[0] = 0;
					if (chooseOrientation[rNumOrientation] == LEFT)
						if (!GoodPoint(generalPoint - generalStep) || generalPoint - generalStep < generalPoint / BOARD_LEN * BOARD_LEN)
							orientation[1] = 0;
					if (chooseOrientation[rNumOrientation] == UP)
						if (!GoodPoint(generalPoint - 10))
							orientation[2] = 0;
					if (chooseOrientation[rNumOrientation] == DOWN)
						if (!GoodPoint(generalPoint + 10))
							orientation[3] = 0;
				}
				int lastPoint = generalPoint;
				if (chooseOrientation.size() != 1)
				{
					if (!chooseOrientation.empty()) chooseOrientation.clear();
					// создаем заново вектор возможных направлений
					for (int i = 0; i < 4; i++) if (orientation[i] != 0) chooseOrientation.push_back(orientation[i]);
					rNumOrientation = rand() % chooseOrientation.size();
				}
				if (chooseOrientation[rNumOrientation] == RIGHT) 
					while (m_human.GetSymbolFromBoard(generalPoint) == HIT) generalPoint++;
				else if (chooseOrientation[rNumOrientation] == LEFT) 
					while (m_human.GetSymbolFromBoard(generalPoint) == HIT) generalPoint--;
				else if (chooseOrientation[rNumOrientation] == UP) 
					while (m_human.GetSymbolFromBoard(generalPoint) == HIT) generalPoint -= 10;
				else if (chooseOrientation[rNumOrientation] == DOWN) 
					while (m_human.GetSymbolFromBoard(generalPoint) == HIT) generalPoint += 10;

				std::cout << "\n\n    > Bot will hit a cell: " << board::PointToString(generalPoint) << "\n ---> ";
				Console::Pause();

				if (m_human.GetAdressFromBoard(generalPoint) != 0)
				{
					generalStep++;
					obj->Hit(); m_human.SetSymbolOnBoard(generalPoint, HIT);
					DisplayMoving();
				}
				else
				{
					m_human.SetSymbolOnBoard(generalPoint, MISS);
					DisplayMoving();
					generalPoint = lastPoint;
					// не попали - убираем направление (повторяет функцию FindBadOrientation при step=0)
					if (generalStep > 0)
					{
						if (chooseOrientation[rNumOrientation] == RIGHT)     orientation[0] = 0;
						else if (chooseOrientation[rNumOrientation] == LEFT) orientation[1] = 0;
						else if (chooseOrientation[rNumOrientation] == UP)   orientation[2] = 0;
						else if (chooseOrientation[rNumOrientation] == DOWN) orientation[3] = 0;
					}
					return;
				}
			}
			// эта часть выполняется только если уничтожен корабль
			char orient = RIGHT;
			if (generalStep != 0)
			{
				orient = chooseOrientation[rNumOrientation];
				if (orient == RIGHT)     orient = LEFT;
				else if (orient == LEFT) orient = RIGHT;
				else if (orient == UP)   orient = DOWN;
				else if (orient == DOWN) orient = UP;
			}
			Bot::DelNumFromBoard(generalPoint, orient, boardNum, *obj);
			m_human.CountMinus(generalStep);

			orientation = { RIGHT, LEFT, UP, DOWN };
			generalPoint = -1; generalStep = 0;
		}
		else
		{
			m_human.SetSymbolOnBoard(generalPoint, MISS);
			std::cout << "\n\n    > Bot will hit a cell: " << board::PointToString(generalPoint) << "\n ---> ";
			Console::Pause();
			DisplayMoving();

			numFind = find(boardNum.begin(), boardNum.end(), generalPoint);
			if (numFind != boardNum.end()) boardNum.erase(numFind);

			generalPoint = -1; break;
		}
	}
}

void SeaBattle::DisplayBoards()
{
	std::cout << "\n        - BOTs BOARD -  \t       - YOUR BOARD -  \n";
	std::cout << "\n     A B C D E F G H I J\t    A B C D E F G H I J\n";
	for (int i = 0; i < BOARD_LEN; i++)
	{
		std::cout << " " << std::setw(2) << std::right << i << " ";
		for (int j = 0; j < BOARD_LEN; j++) std::cout << " " << m_bot.GetSymbolFromBoard(i * BOARD_LEN + j);
		std::cout << "\t" << std::setw(2) << std::right << i << " ";
		for (int j = 0; j < BOARD_LEN; j++) std::cout << " " << m_human.GetSymbolFromBoard(i * BOARD_LEN + j);
		std::cout << std::endl;
	}
}

void SeaBattle::DisplayMoving(bool human)
{
	Console::Clear();
	if (human) std::cout << "\n | GAME STARTED: HUMAN MOVE\n\n";
	else std::cout << "\n | GAME STARTED: BOT MOVE\n\n";
	std::cout << " |              Ships   Count   |              Ships   Count\n";
	std::cout << " | Bots ships:  S        x" << m_bot.GetCount(0) << "     | Your ships:  S        x" << m_human.GetCount(0) << "\n";
	std::cout << "                SS       x" << m_bot.GetCount(1) << "                    SS       x" << m_human.GetCount(1) << "\n";
	std::cout << "                SSS      x" << m_bot.GetCount(2) << "                    SSS      x" << m_human.GetCount(2) << "\n";
	std::cout << "                SSSS     x" << m_bot.GetCount(3) << "                    SSSS     x" << m_human.GetCount(3) << "\n\n";

	DisplayBoards();
}

void SeaBattle::PreStart()
{
	std::cout << "\n | LOADING GAME...\n";
	std::cout << "\n | CREATOR: MARIA MOLCHANOVA\n\n\n";


	std::cout << "            |. .\n";
	std::cout << "            | .   .\n";
	std::cout << "            |  . .    .\n";
	std::cout << "            |   .  .  .   .\n";
	std::cout << "            |   .  .  .  .\n";
	std::cout << "            |  . .    .\n";
	std::cout << "            | .   .\n";
	std::cout << "            |. .\n";
	std::cout << "  _____________________________\n";
	std::cout << "  \\#  #  ####      ##    # ###/ \n";
	std::cout << "   \\##   ##  ###  #    #   ##/ \n";
	std::cout << "    \\   ##    #  ###  ###   / \n";
	std::cout << "  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n | ";

	Console::Pause();
	DisplayRules();

	std::string input;
	std::cout << "\n\n\n ---> Enter 1 to start putting ships: ";
	Console::Read(input);
	if (input == "1") return;
	else
	{
		DisplayRules();
		std::cout << "\n\n    > You dont enter the 1!";
		std::cout << "\n ---> Enter 1 to start putting ships: ";
		Console::Read(input);
		if (input == "1") return;
		else 
		{
			DisplayRules();
			std::cout << "\n\n    > Ok, whatever...\n";
			return;
		}
	}
}

void SeaBattle::DisplayRules()
{
	Console::Clear();
	std::cout << "\n | PRESTART GAME\n\n";
	std::cout << "\n                     --- SEABATTLE ---\n";
	std::cout << "\n | Welcome to game: SeaBattle!";
	std::cout << "\n | Read the rules for playing (not the game rules):\n";
	std::cout << "\n        - YOUR BOARD -  \n";
	std::cout << "\n     A B C D E F G H I J";
	std::cout << "\n  0  + + + + + + + + + +   > You have: 4 x S, 3 x SS, 2 x SSS, 1 x SSSS ships.";
	std::cout << "\n  1  + + + + + + + + + +   > To put ship you need to choose the ship.";
	std::cout << "\n  2  + + S + + + + + + +      Then write cell code like: 0A> or 0a>.";
	std::cout << "\n  3  + + + + + + + S + +   > You can use 4 orientations: >(right), <(left), ^(up), v(down).";
	std::cout << "\n  4  + + + + + + + S + +   > By defolt orientation is > so you can to not write orientation.";
	std::cout << "\n  5  + + + + + + + S + +";
	std::cout << "\n  6  + + + + + + + S + +     Examples (look on the board):";
	std::cout << "\n  7  + S S S + + + + + +   > First, I choose ship #1 (S) and then write: 2C.";
	std::cout << "\n  8  + + + + + + + + + +   > I choose ship #4 (SSSS) and write: 6h^.";
	std::cout << "\n  9  + + + + + + + + + +   > I choose ship #3 (SSS) and write: 7B.";
}

void SeaBattle::End()
{
	Console::Clear();
	std::cout << "\n | ENDING GAME";
	std::cout << "\n\n | GAME OVER:";

	if (m_human.EmptyShip())
	{
		for (int point = 0; point < BOARD_LEN * BOARD_LEN; point++)
			if (m_bot.GetSymbolFromBoard(point) == GAME_SYMBOL && m_bot.GetAdressFromBoard(point) != 0)
				m_bot.SetSymbolOnBoard(point, SHIP_SYMBOL);
		std::cout << "\n | BOT WINS!\n\n";
	}
	else std::cout << "\n | YOU WIN!\n\n";
	std::cout << "\n | See the ships you cant find (symbol: S):\n\n";
	DisplayBoards();
	std::cout << "\n\n\n\n\n\n ";
}

void SeaBattle::Error(int errNum)
{
	DisplayMoving(true);
	switch (errNum)
	{
	case 6:
		std::cout << "\n   !> Error: the cell has already been used.";
		break;
	case 7:
		std::cout << "\n   !> Error: size less than 2.";
		break;
	case 8:
		std::cout << "\n   !> Error: first symbol is not number.";
		break;
	case 9:
		std::cout << "\n   !> Error: second symbol should be a letter from board columns.";
		break;
	}
}