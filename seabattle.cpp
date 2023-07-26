#include "seabattle.h"

SeaBattle::SeaBattle() : m_bHuman(), m_bBot()
{
	ship OD_1(1), OD_2(1), OD_3(1), OD_4(1);
	ship TD_1(2), TD_2(2), TD_3(2);
	ship FD_1(3), FD_2(3), FoD(4);
	std::vector<ship> ships = { OD_1, OD_2, OD_3, OD_4, TD_1, TD_2, TD_3, FD_1, FD_2, FoD };
	
	m_human = ships;
	m_bot = ships;

	for (int i = 4; i > 0; i--) m_humanShipCount[4 - i] = i;
	for (int i = 4; i > 0; i--) m_botShipCount[4 - i] = i;
}

void SeaBattle::RemakeBoardsForStart()
{
	m_bHuman.SetStep(0); m_bBot.SetStep(0);
	for (int point = 0; point < BOARD_LEN * BOARD_LEN; point++)
	{
		m_bBot.SetSymbolPoint(point, GAME_SYMBOL);
		if (m_bHuman.GetSymbolPoint(point) == CLOSE_SYMBOL || m_bHuman.GetSymbolPoint(point) == START_SYMBOL) 
			m_bHuman.SetSymbolPoint(point, GAME_SYMBOL);
		else m_bHuman.m_steps[point] = 0;
	}
	m_bHuman.SetStep(1); m_bBot.SetStep(1);
	for (int i = 4; i > 0; i--) m_humanShipCount[4 - i] = i;
	for (int i = 4; i > 0; i--) m_botShipCount[4 - i] = i;
}

ship* SeaBattle::GetHumanShipAdress(int number)
{
	if (number == 1) return &m_human[4 - m_humanShipCount[0]];
	else if (number == 2) return &m_human[7 - m_humanShipCount[1]];
	else if (number == 3) return &m_human[9 - m_humanShipCount[2]];
	else return &m_human[9];
}

ship* SeaBattle::GetBotShipAdress(int number)
{
	if (number == 0) return &m_bot[4 - m_botShipCount[0]];
	else if (number == 1) return &m_bot[7 - m_botShipCount[1]];
	else if (number == 2) return &m_bot[9 - m_botShipCount[2]];
	else return &m_bot[9];
}

void SeaBattle::Start()
{
	PreStart();
	//return;
	BotPutsShip();
	HumanPutsShip();
	//return;
	RemakeBoardsForStart();
	//g.m_bHuman.DisplayBoards();
	while (!EmptyHumanShip() && !EmptyBotShip())
	{
		HumanMove();
		BotMove();
	}
	return;
	for (int i = 0; i < 100; i++)
	{
		HumanMove();
		BotMove();
		//g.m_bHuman.DisplayBoards();
		//Sleep(7000);
	}
	return;
}

bool SeaBattle::EmptyHumanShip()
{
	if (m_humanShipCount[0] + m_humanShipCount[1] + m_humanShipCount[2] + m_humanShipCount[3] == 9)
		return true;
	return false;
}

bool SeaBattle::EmptyBotShip()
{
	if (m_botShipCount[0] + m_botShipCount[1] + m_botShipCount[2] + m_botShipCount[3] == 0)
		return true;
	return false;
}

void SeaBattle::HumanPutsShip()
{
	bool PutShip = true;
	std::string input; int err_count = 0, number = 1;
	ship* p_ship = 0;

	int a = 0, deck = 0;
	while (true)
	{
		PutDisplay();
		if (EmptyHumanShip()) break;

		err_count = 0;
		std::cout << "\n\n     (Enter <back> to cancel last step)";
		std::cout << "\n ---> Enter ship number: ";
		std::cin >> input;

		if (input == "back" || input == "b")
		{ 
			deck = m_bHuman.DeleteLastShip();
			if (deck == 1) m_humanShipCount[0]++;
			else if (deck == 2) m_humanShipCount[1]++;
			else if (deck == 3) m_humanShipCount[2]++;
			else if (deck == 4) m_humanShipCount[3]++;
			else break;
			continue; 
		}

		while (!IsCorrectNum(input))
		{
			err_count++;
			std::cout << "\n     (Enter <back> to cancel last step)";

			if (err_count % 3 != 0)
			{
				std::cout << "\n ---> Enter ship number (example: 1): ";
			}
			else std::cout << "\n      Are you sure you want to play?\n ---> Enter ship number or <back>: ";
			std::cin >> input;
			if (input == "back" || input == "b") { break; }
		}
		if (input == "back" || input == "b")
		{
			deck = m_bHuman.DeleteLastShip();
			if (deck == 1) m_humanShipCount[0]++;
			else if (deck == 2) m_humanShipCount[1]++;
			else if (deck == 3) m_humanShipCount[2]++;
			else m_humanShipCount[3]++;
			continue;
		}
		number = input[0] - 48;
		p_ship = GetHumanShipAdress(number);

		err_count = 0;
		std::cout << "\n\n    > Selected ship: "; for (int i = 0; i < number; i++) std::cout << "S";
		std::cout << " (Enter <back> to cancel selection)";
		std::cout << "\n ---> Enter cell number: ";
		std::cin >> input;

		if (input == "back" || input == "b") { p_ship = 0; continue; }

		while (!IsInputCorrect(input, *p_ship, m_bHuman))
		{
			err_count++;
			std::cout << "\n    > Selected ship: "; for (int i = 0; i < number; i++) std::cout << "S";
			std::cout << " (Enter <back> to cancel selection)";
			if (err_count % 5 != 0) std::cout << "\n ---> Enter cell number (example: 1A>): ";
			else std::cout << "\n      Are you sure you want to play?\n ---> Enter cell number: ";
			std::cin >> input;
			if (input == "back" || input == "b") break;
		}
		if (input == "back" || input == "b") { p_ship = 0; continue; }

		m_bHuman.PutShip(input, *p_ship);
		//std::cout << &m_human[0] << " " << p_ship << " " << m_bHuman.p_board[0];
		m_humanShipCount[number - 1]--;

		a++;
	}
}

bool SeaBattle::IsInputCorrect(std::string& input, const ship& obj, const board& b)
{
	if (size(input) < 2) { Error(0); return false; }												// если длина < 2, то плохо
	char check = input[0];
	if (check < '0' || check > '9') { Error(1); return false; }									// если НЕ между 0 и 9, то ай-яй
	check = input[1];
	if (check < 'A' || check > 'J') if (check < 'a' || check > 'j') { Error(2); return false; }		// если НЕ между A и J или НЕ между a и j, то ой-ёй
	if (obj.GetDeck() != 1)																// если НЕ однопалубный
	{
		if (size(input) >= 3)
		{
			check = input[2];
			if (check == RIGHT || check == LEFT || check == UP || check == DOWN);	// если равно хотя бы одному, то молодец
			else { Error(3); return false; }
		}
		else input += RIGHT;																// по умолчанию будет >
	}

	int firstElOfRow = (input[0] - 48) * BOARD_LEN, point = firstElOfRow; check = input[1];
	if (check < 75) point += input[1] - 65; else point += input[1] - 97;

	if (obj.GetDeck() == 1) 
		if (b.GetSymbolPoint(point) == CLOSE_SYMBOL || b.GetSymbolPoint(point) == SHIP_SYMBOL) { Error(5); return false; }
		else return true;

	char orientation = input[2];
	if (orientation == RIGHT) if ((point + (obj.GetDeck() - 1)) > firstElOfRow + 9) { Error(4); return false; }
	if (orientation == LEFT) if ((point - (obj.GetDeck() - 1)) < firstElOfRow) { Error(4); return false; }
	if (orientation == UP) if ((point - BOARD_LEN * (obj.GetDeck() - 1)) < 0) { Error(4); return false; }
	if (orientation == DOWN) if ((point + BOARD_LEN * (obj.GetDeck() - 1)) > 99) { Error(4); return false; }

	if (orientation == RIGHT) for (int i = point; i < point + obj.GetDeck(); i++)
		if (b.GetSymbolPoint(i) == CLOSE_SYMBOL) { Error(5); return false; }
	if (orientation == LEFT) for (int i = point; i > point - obj.GetDeck(); i--)
		if (b.GetSymbolPoint(i) == CLOSE_SYMBOL) { Error(5); return false; }
	if (orientation == UP) for (int i = point; i > point - obj.GetDeck() * BOARD_LEN; i -= BOARD_LEN)
		if (b.GetSymbolPoint(i) == CLOSE_SYMBOL) { Error(5); return false; }
	if (orientation == DOWN) for (int i = point; i < point + obj.GetDeck() * BOARD_LEN; i += BOARD_LEN)
		if (b.GetSymbolPoint(i) == CLOSE_SYMBOL) { Error(5); return false; }

	return true;
}

bool SeaBattle::IsCorrectNum(std::string& input)
{
	PutDisplay();
	if (size(input) < 1) return false;
	int check = input[0] - 48;
	if (check < 1 || check > 4)
	{
		std::cout << "\n   !> Error: number should be in [1,4].";
		return false;
	}

	if (m_humanShipCount[check - 1] == 0)
	{
		std::cout << "\n   !> Error: count of ship #" << check << " = 0.";
		return false;
	}
	return true;
}

void SeaBattle::DisplayBoards()
{
	std::cout << "\n        - BOTs BOARD -  \t       - YOUR BOARD -  \n";
	std::cout << "\n     A B C D E F G H I J\t    A B C D E F G H I J\n";
	for (int i = 0; i < BOARD_LEN; i++)
	{
		std::cout << " " << std::setw(2) << std::right << i << " ";
		for (int j = 0; j < BOARD_LEN; j++) std::cout << " " << m_bBot.GetSymbolPoint(i * BOARD_LEN + j);
		std::cout << "\t" << std::setw(2) << std::right << i << " ";
		for (int j = 0; j < BOARD_LEN; j++) std::cout << " " << m_bHuman.GetSymbolPoint(i * BOARD_LEN + j);
		std::cout << std::endl;
	}
}

void DelNumFromBoard(int point, char orientation, std::vector<int>& numbers, ship& obj)
{
	int leftUpPoint, lenght = 0, weight = 0;
	if (orientation == LEFT) point -= obj.GetDeck() - 1;
	else if (orientation == UP) point -= (obj.GetDeck() - 1) * 10;
	leftUpPoint = point;
	if (point > 9) leftUpPoint -= 10;
	if (point % 10 > 0) leftUpPoint -= 1;

	if (orientation == RIGHT || orientation == LEFT)
	{
		lenght = obj.GetDeck() + 2; weight = 3;
		if (point % 10 == 0) lenght -= 1;
		else if (point % 10 + obj.GetDeck() >= 10) lenght -= 1;
	}
	if (orientation == UP || orientation == DOWN)
	{
		lenght = 3; weight = obj.GetDeck() + 2;
		if (point % 10 == 0) lenght -= 1;
		else if (point % 10 + 1 >= 10) lenght -= 1;
	}

	if (point < 10) weight -= 1;
	else if (point >= 90) weight -= 1;

	std::vector<int>::iterator numFind;

	for (int w = 0; w < weight; w++)
	{
		for (int l = 0; l < lenght; l++)
		{
			numFind = find(numbers.begin(), numbers.end(), leftUpPoint + l);
			if (numFind != numbers.end()) numbers.erase(numFind);
		}
		leftUpPoint += 10;
	}
}

void SeaBattle::BotPutsShip()
{
	//m_bBot.DisplayBoard();

	const std::vector<char> orientation = { RIGHT, LEFT, UP, DOWN };						// все направления
	std::vector<char> chooseOrientation;												// выбранные направления (возможные)
	std::vector<int> shipNumber; for (int i = 0; i < 4; i++) shipNumber.push_back(i);	// номер корабля, НЕ сам корабль
	std::vector<int> boardNum; for (int i = 0; i < 100; i++) boardNum.push_back(i);		// точки доски от 0 до 100

	srand(static_cast<unsigned int>(time(0)));
	int rNumShip = 0;// rand() % shipNumber.size();		// рандомный номер корабля
	int rNumBoard = 0;								// рандомный номер точки доски
	int rNumOrientation = 0;						// рандомный номер возможных направлений

	std::string input;
	ship* p_ship = 0;								// указатель на корабль

	int cat = 0, cat2 = 0, step = 0, deck = 0;
	while (!shipNumber.empty())
	{
		cat++; if (cat == 100) break;

		rNumShip = rand() % shipNumber.size();
		rNumShip = shipNumber[rNumShip];
		p_ship = GetBotShipAdress(rNumShip);

		// если было 3 попытки поставить корабль - удалить последний поставленный
		if (step == 3)
		{
			deck = m_bBot.DeleteLastShip(boardNum);
			if (deck == 1) { m_botShipCount[0]++; if (m_botShipCount[0] == 1) shipNumber.push_back(0); }
			else if (deck == 2) { m_botShipCount[1]++; if (m_botShipCount[1] == 1) shipNumber.push_back(1); }
			else if (deck == 3) { m_botShipCount[2]++; if (m_botShipCount[2] == 1) shipNumber.push_back(2); }
			else if (deck == 4) { m_botShipCount[3]++; if (m_botShipCount[3] == 1) shipNumber.push_back(3); }
			step = 0; cat2++;
		}

		rNumBoard = rand() % boardNum.size(); rNumBoard = boardNum[rNumBoard];

		// если не однопалубный - найти возможные направления
		if (p_ship->GetDeck() != 1)
		{
			step++;
			// найти направления, вмещающиеся в строку и/или не выходящие за границы доски
			if ((rNumBoard + (p_ship->GetDeck() - 1)) > (rNumBoard / 10) * 10 + 9); else chooseOrientation.push_back(orientation[0]);
			if ((rNumBoard - (p_ship->GetDeck() - 1)) < (rNumBoard / 10) * 10); else chooseOrientation.push_back(orientation[1]);
			if ((rNumBoard - BOARD_LEN * (p_ship->GetDeck() - 1)) < 0); else chooseOrientation.push_back(orientation[2]);
			if ((rNumBoard + BOARD_LEN * (p_ship->GetDeck() - 1)) > 99); else chooseOrientation.push_back(orientation[3]);
			// найти направления из уже подходящих, которые НЕ пересекают границы др. кораблей
			for (int i = rNumBoard; i < rNumBoard + p_ship->GetDeck(); i++) if (m_bBot.GetSymbolPoint(i) == CLOSE_SYMBOL)
				for (int j = 0; j < chooseOrientation.size(); j++) if (chooseOrientation[j] == RIGHT) chooseOrientation.erase(chooseOrientation.begin() + j);
			for (int i = rNumBoard; i > rNumBoard - p_ship->GetDeck(); i--) if (m_bBot.GetSymbolPoint(i) == CLOSE_SYMBOL)
				for (int j = 0; j < chooseOrientation.size(); j++) if (chooseOrientation[j] == LEFT) chooseOrientation.erase(chooseOrientation.begin() + j);
			for (int i = rNumBoard; i > rNumBoard - p_ship->GetDeck() * BOARD_LEN; i -= BOARD_LEN) if (m_bBot.GetSymbolPoint(i) == CLOSE_SYMBOL)
				for (int j = 0; j < chooseOrientation.size(); j++) if (chooseOrientation[j] == UP) chooseOrientation.erase(chooseOrientation.begin() + j);
			for (int i = rNumBoard; i < rNumBoard + p_ship->GetDeck() * BOARD_LEN; i += BOARD_LEN) if (m_bBot.GetSymbolPoint(i) == CLOSE_SYMBOL)
				for (int j = 0; j < chooseOrientation.size(); j++) if (chooseOrientation[j] == DOWN) chooseOrientation.erase(chooseOrientation.begin() + j);
			// если нет таких направлений - заново, иначе выбрать рандомно направление, удалить точки доски
			if (chooseOrientation.empty()) continue;
			else
			{
				rNumOrientation = rand() % chooseOrientation.size();
				DelNumFromBoard(rNumBoard, chooseOrientation[rNumOrientation], boardNum, *p_ship);
				step = 0;
			}
		}
		else DelNumFromBoard(rNumBoard, RIGHT, boardNum, *p_ship);
		// составить код точки для функции, поставить корабль
		input = board::PointToString(rNumBoard);
		//input += char(rNumBoard / 10 + 48);
		//input += char(rNumBoard % 10 + 65);
		if (p_ship->GetDeck() != 1) input += chooseOrientation[rNumOrientation];
		m_bBot.PutShip(input, *p_ship);
		m_botShipCount[rNumShip]--;
		if (m_botShipCount[rNumShip] == 0) for (int i = 0; i < shipNumber.size(); i++)
			if (shipNumber[i] == rNumShip) shipNumber.erase(shipNumber.begin() + i);

		//m_bBot.DisplayBoards();
		// если есть корабли - выбрать новый корабль и очистить направления
		/*if (!shipNumber.empty())
		{
			rNumShip = rand() % shipNumber.size();
			rNumShip = shipNumber[rNumShip];
			p_ship = GetBotShipAdress(rNumShip);
		}*/
		input = "";
		chooseOrientation.clear();
	}
	//m_bBot.DisplayBoards();
	std::cout << "\n cat = " << cat << " cat2 = " << cat2 << std::endl;
	return;
}

bool SeaBattle::CellCorrect(std::string& input)
{
	if (size(input) < 2) { Error(0); return false; }												// если длина < 2, то плохо
	char check = input[0];
	if (check < '0' || check > '9') { Error(1); return false; }									// если НЕ между 0 и 9, то ай-яй
	check = input[1];
	if (check < 'A' || check > 'J') if (check < 'a' || check > 'j') { Error(2); return false; }		// если НЕ между A и J или НЕ между a и j, то ой-ёй

	int point = board::StringToPoint(input);
	if (m_bBot.GetSymbolPoint(point) != GAME_SYMBOL) { Error(6); return false; }
	return true;
}

void SeaBattle::HumanMove()
{
	std::string input; int point = 0, count = 0, err_count = 0;

	while (!EmptyBotShip())
	{
		MoveDisplay(true);

		std::cout << "\n\n ---> Enter cell number: ";
		std::cin >> input;

		err_count = 0;
		while (!CellCorrect(input))
		{
			err_count++;
			MoveDisplay(true);
			if (err_count % 5 != 0) std::cout << "\n\n ---> Enter cell number (example: 1A): ";
			else std::cout << "\n      So boring... Bot fell asleep.\n ---> Enter cell number: ";
			std::cin >> input;
		}
		point = board::StringToPoint(input);

		if (m_bBot.GetAdress(point) != 0)
		{
			ship* obj = m_bBot.GetAdress(point); count = 0;
			obj->Hit();
			if (obj->ShipDestroyed()) for (int i = 0; i < BOARD_LEN * BOARD_LEN; i++)
			{
				if (m_bBot.GetAdress(i) == obj)
				{
					m_bBot.SetSymbolPoint(i, SHIP_SYMBOL);
					count++; if (count == obj->GetDeck()) break;
				}
			}
			else m_bBot.SetSymbolPoint(point, HIT);
			if (obj->ShipDestroyed()) m_botShipCount[obj->GetDeck() - 1]--;
		}
		else
		{
			m_bBot.SetSymbolPoint(point, MISS);
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

void SeaBattle::BotMove()
{
	static std::vector<char> orientation = { RIGHT, LEFT, UP, DOWN };						// все направления
	std::vector<char> chooseOrientation;												// выбранные направления (возможные)
	static std::vector<int> boardNum; if (boardNum.empty()) for (int i = 0; i < 100; i++) boardNum.push_back(i);
	static int point = -1; static int step = 0;
	static bool test = true;
	
	srand(static_cast<unsigned int>(time(0)));
	int rNumBoard = 0;								// рандомный номер точки доски
	int rNumOrientation = 0;						// рандомный номер возможных направлений

	std::vector<int>::iterator numFind;

	while (!EmptyHumanShip())
	{
		MoveDisplay(false);
		if (point == -1)
		{
			if (test)
			{
				rNumBoard = 22; test = false;
			}
			else rNumBoard = rand() % boardNum.size();
			rNumBoard = boardNum[rNumBoard];
			point = rNumBoard;
		}

		// если попал !!!
		if (m_bHuman.GetAdress(point) != 0)
		{
			ship* obj = m_bHuman.GetAdress(point);
			if (GoodPoint(point)) 
			{ 
				obj->Hit(); 
				m_bHuman.SetSymbolPoint(point, HIT); 
				std::cout << "\n\n    > Bot will hit a cell: " << board::PointToString(point) << "\n ---> ";
				system("pause");
				MoveDisplay(false); 
			}

			while (!obj->ShipDestroyed())
			{
				if (step == 0) FindBadOrientations(point, 1, orientation);
				if (step == 1) if (chooseOrientation.size() > 1)
					if (chooseOrientation[rNumOrientation] == RIGHT || chooseOrientation[rNumOrientation] == LEFT)
					{
						orientation[2] = 0; orientation[3] = 0;
					}
					else
					{
						orientation[0] = 0; orientation[1] = 0;
					}
				if (step > 0) if (chooseOrientation.size() > 1)
				{
					if (chooseOrientation[rNumOrientation] == RIGHT)
						if (!GoodPoint(point + step) || point + step > point / BOARD_LEN * BOARD_LEN + 9) orientation[0] = 0;
					if (chooseOrientation[rNumOrientation] == LEFT)
						if (!GoodPoint(point - step) || point - step < point / BOARD_LEN * BOARD_LEN) orientation[1] = 0;
					if (chooseOrientation[rNumOrientation] == UP)
						if (!GoodPoint(point - 10)) orientation[2] = 0;
					if (chooseOrientation[rNumOrientation] == DOWN)
						if (!GoodPoint(point + 10)) orientation[3] = 0;
				}
				int lastPoint = point;
				if (chooseOrientation.size() != 1)
				{
					if (!chooseOrientation.empty()) chooseOrientation.clear();

					for (int i = 0; i < 4; i++) if (orientation[i] != 0) chooseOrientation.push_back(orientation[i]);
					rNumOrientation = rand() % chooseOrientation.size();
				}
				if (chooseOrientation[rNumOrientation] == RIGHT) while (m_bHuman.GetSymbolPoint(point) == HIT) point = point + 1;
				else if (chooseOrientation[rNumOrientation] == LEFT) while (m_bHuman.GetSymbolPoint(point) == HIT) point = point - 1;
				else if (chooseOrientation[rNumOrientation] == UP) while (m_bHuman.GetSymbolPoint(point) == HIT) point = point - 10;
				else if (chooseOrientation[rNumOrientation] == DOWN) while (m_bHuman.GetSymbolPoint(point) == HIT) point = point + 10;

				std::cout << "\n\n    > Bot will hit a cell: " << board::PointToString(point) << "\n ---> ";
				system("pause");

				if (m_bHuman.GetAdress(point) != 0)
				{
					step++;
					obj->Hit(); m_bHuman.SetSymbolPoint(point, HIT);
					MoveDisplay(false);
				}
				else
				{
					m_bHuman.SetSymbolPoint(point, MISS);
					MoveDisplay(false);
					point = lastPoint;
					if (step > 0)
					{
						if (chooseOrientation[rNumOrientation] == RIGHT)
						{
							//while (m_bHuman.GetSymbolPoint(point) == 'V') point = point - 1;
							orientation[0] = 0;
						}
						else if (chooseOrientation[rNumOrientation] == LEFT)
						{
							//while (m_bHuman.GetSymbolPoint(point) == 'V') point = point + 1;
							orientation[1] = 0;
						}
						else if (chooseOrientation[rNumOrientation] == UP)
						{
							//while (m_bHuman.GetSymbolPoint(point) == 'V') point = point + 10;
							orientation[2] = 0;
						}
						else if (chooseOrientation[rNumOrientation] == DOWN)
						{
							//while (m_bHuman.GetSymbolPoint(point) == 'V') point = point - 10;
							orientation[3] = 0;
						}
					}
					return;
				}
			}
			char orient = RIGHT;
			if (step != 0)
			{
				orient = chooseOrientation[rNumOrientation];
				if (orient == RIGHT) orient = LEFT;
				else if (orient == LEFT) orient = RIGHT;
				else if (orient == UP) orient = DOWN;
				else if (orient == DOWN) orient = UP;
			}
			DelNumFromBoard(point, orient, boardNum, *obj);
			m_humanShipCount[step]--;

			orientation = { RIGHT, LEFT, UP, DOWN };
			point = -1; step = 0;
		}
		else
		{
			m_bHuman.SetSymbolPoint(point, MISS);
			std::cout << "\n\n    > Bot will hit a cell: " << board::PointToString(point) << "\n ---> ";
			system("pause");
			MoveDisplay(false);
			numFind = find(boardNum.begin(), boardNum.end(), point);
			if (numFind != boardNum.end()) boardNum.erase(numFind);
			point = -1;
			break;
		}
	}
}

bool SeaBattle::GoodPoint(int point)
{
	if (point < 0 || point >= BOARD_LEN * BOARD_LEN) return false;
	if (m_bHuman.m_steps[point] != 0) return false;
	return true;
}

void SeaBattle::MoveDisplay(bool human)
{
	system("cls");
	if (human) std::cout << "\n | GAME STARTED: HUMAN MOVE\n\n";
	else std::cout << "\n | GAME STARTED: BOT MOVE\n\n";
	std::cout << " |              Ships   Count   |              Ships   Count\n";
	std::cout << " | Bots ships:  S        x" << m_botShipCount[0] << "     | Your ships:  S        x" << m_humanShipCount[0] << "\n";
	std::cout << "                SS       x" << m_botShipCount[1] << "                    SS       x" << m_humanShipCount[1] << "\n";
	std::cout << "                SSS      x" << m_botShipCount[2] << "                    SSS      x" << m_humanShipCount[2] << "\n";
	std::cout << "                SSSS     x" << m_botShipCount[3] << "                    SSSS     x" << m_humanShipCount[3] << "\n\n";

	DisplayBoards();
}

void SeaBattle::PutDisplay()
{
	system("cls");
	std::cout << "\n | PUTTING SHIPS...\n\n";
	std::cout << " |            Number   Ships   Count\n";
	std::cout << " | You have:    1       S       x" << m_humanShipCount[0] << "\n";
	std::cout << "                2       SS      x" << m_humanShipCount[1] << "\n";
	std::cout << "                3       SSS     x" << m_humanShipCount[2] << "\n";
	std::cout << "                4       SSSS    x" << m_humanShipCount[3] << "\n";

	m_bHuman.DisplayBoard();
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

	system("pause");
	DisplayRules();

	std::string input = "";
	std::cout << "\n\n\n ---> Enter 1 to start putting ships: ";
	std::cin >> input;
	if (input == "1") return;
	else
	{
		DisplayRules();
		std::cout << "\n\n    > You dont enter the 1!";
		std::cout << "\n ---> Enter 1 to start putting ships: ";
		std::cin >> input;
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
	system("cls");
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
	system("cls");
	std::cout << "\n | ENDING GAME\n\n";
	std::cout << "\n\n | GAME OVER!\n\n";

	if (EmptyHumanShip())
	{
		for (int point = 0; point < BOARD_LEN * BOARD_LEN; point++)
			if (m_bHuman.GetSymbolPoint(point) == GAME_SYMBOL && m_bHuman.GetAdress(point) != 0)
				m_bHuman.SetSymbolPoint(point, SHIP_SYMBOL);
		std::cout << "\n\n | BOT WINS!\n\n";
	}
	else std::cout << "\n\n | YOU WIN!\n\n";
	DisplayBoards();
}

void SeaBattle::Error(int errNum)
{
	PutDisplay();
	switch (errNum)
	{
	case 0:
		std::cout << "\n   !> Error: size less than 2.";
		break;
	case 1:
		std::cout << "\n   !> Error: first symbol is not number.";
		break;
	case 2:
		std::cout << "\n   !> Error: second symbol should be a letter from board columns.";
		break;
	case 3:
		std::cout << "\n   !> Error: third symbol is incorrect. Use: > left, < right, ^ up, v down).";
		break;
	case 4:
		std::cout << "\n   !> Error: ship can not set here.";
		break;
	case 5:
		std::cout << "\n   !> Error: ship crosses other ship.";
		break;
	case 6:
		std::cout << "   !> Error: the cell has already been used.";
		break;
	}
}