#include "player.h"

Player::Player() : m_board()
{
	ship OD_1(1), OD_2(1), OD_3(1), OD_4(1);
	ship TD_1(2), TD_2(2), TD_3(2);
	ship ThD_1(3), ThD_2(3), FD(4);
	m_ships.reserve(10);
	m_ships = { OD_1, OD_2, OD_3, OD_4, TD_1, TD_2, TD_3, ThD_1, ThD_2, FD };

	for (int i = 4; i > 0; i--) m_shipCount[4 - i] = i;
}

char Player::GetSymbolFromBoard(const int point)
{
	return m_board.GetSymbolPoint(point);
}

int Player::GetStepFromBoard(const int point)
{
	return m_board.GetStepBoard(point);
}

ship* Player::GetAdressFromBoard(const int point)
{
	return m_board.GetAdress(point);
}

ship* Player::GetShipAdress(int number)
{
	switch (number)
	{
	case 0:
		return &m_ships[static_cast<size_t>(4) - static_cast<size_t>(m_shipCount[0])];
	case 1:
		return &m_ships[static_cast<size_t>(7) - static_cast<size_t>(m_shipCount[1])];
	case 2:
		return &m_ships[static_cast<size_t>(9) - static_cast<size_t>(m_shipCount[2])];
	case 3:
		return &m_ships[static_cast<size_t>(9)];
	}
}

int Player::GetCount(const int number) const
{
	return m_shipCount[number];
}

void Player::SetSymbolOnBoard(const int point, char symbol)
{
	m_board.SetSymbolPoint(point, symbol);
}

void Player::SetStepOnBoard(const int point, const int step)
{
	m_board.SetStepBoard(point, step);
}

void Player::SetGeneralStep(const int step)
{
	m_board.SetStep(step);
}

bool Player::EmptyShip() const
{
	if (m_shipCount[0] + m_shipCount[1] + m_shipCount[2] + m_shipCount[3] == 0)
		return true;
	return false;
}

void Player::CountMinus(const int number)
{
	m_shipCount[number]--;
}

void Player::CountPlus(const int number)
{
	m_shipCount[number]++;
}

void Player::ResetCount()
{
	for (int i = 4; i > 0; i--) m_shipCount[4 - i] = i;
}



Human::Human() : Player()
{}

void Human::PutsShip()
{
	bool PutShip = true;
	std::string input; int err_count = 0, number = 1;
	ship* p_ship = 0;

	int a = 0, deck = 0;
	while (true)
	{
		DisplayForPutting();
		if (EmptyShip()) break;

		err_count = 0;
		std::cout << "\n\n     (Enter <back> to cancel last step)";
		std::cout << "\n ---> Enter ship number: ";
		Console::Read(input);

		if (input == "back" || input == "b")
		{
			deck = m_board.DeleteLastShip();
			if (deck > 0) CountPlus(deck - 1);
			else Console::QuitProgram();
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
			Console::Read(input);
			if (input == "back" || input == "b") { break; }
		}
		if (input == "back" || input == "b")
		{
			deck = m_board.DeleteLastShip();
			CountPlus(deck - 1);
			continue;
		}
		number = input[0] - 48;
		p_ship = GetShipAdress(number - 1);

		err_count = 0;
		std::cout << "\n\n    > Selected ship: "; for (int i = 0; i < number; i++) std::cout << "S";
		std::cout << " (Enter <back> to cancel selection)";
		std::cout << "\n ---> Enter cell number: ";
		Console::Read(input);

		if (input == "back" || input == "b") { p_ship = 0; continue; }

		while (!IsInputCorrect(input, *p_ship, m_board))
		{
			err_count++;
			std::cout << "\n    > Selected ship: "; for (int i = 0; i < number; i++) std::cout << "S";
			std::cout << " (Enter <back> to cancel selection)";
			if (err_count % 5 != 0) std::cout << "\n ---> Enter cell number (example: 1A>): ";
			else std::cout << "\n      Are you sure you want to play?\n ---> Enter cell number: ";
			Console::Read(input);
			if (input == "back" || input == "b") break;
		}
		if (input == "back" || input == "b") { p_ship = 0; continue; }

		m_board.PutShip(input, *p_ship);
		CountMinus(number - 1);

		a++;
	}
}

bool Human::IsInputCorrect(std::string& input, const ship& obj, const board& b)
{
	if (size(input) < 2) { Error(0); return false; }
	char check = input[0];
	if (check < '0' || check > '9') { Error(1); return false; }
	check = input[1];
	if (check < 'A' || check > 'J') if (check < 'a' || check > 'j') { Error(2); return false; }
	if (obj.GetDeck() != 1)
	{
		if (size(input) >= 3)
		{
			check = input[2];
			if (check == RIGHT || check == LEFT || check == UP || check == DOWN);
			else { Error(3); return false; }
		}
		else input += RIGHT;
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

bool Human::IsCorrectNum(std::string& input)
{
	DisplayForPutting();
	if (size(input) < 1) return false;
	int check = input[0] - 48;
	if (check < 1 || check > 4)
	{
		std::cout << "\n   !> Error: number should be in [1,4].";
		return false;
	}

	if (m_shipCount[check - 1] == 0)
	{
		std::cout << "\n   !> Error: count of ship #" << check << " = 0.";
		return false;
	}
	return true;
}

void Human::DisplayForPutting()
{
	Console::Clear();
	std::cout << "\n | PUTTING SHIPS...\n\n";
	std::cout << " |            Number   Ships   Count\n";
	std::cout << " | You have:    1       S       x" << m_shipCount[0] << "\n";
	std::cout << "                2       SS      x" << m_shipCount[1] << "\n";
	std::cout << "                3       SSS     x" << m_shipCount[2] << "\n";
	std::cout << "                4       SSSS    x" << m_shipCount[3] << "\n";

	m_board.DisplayBoard();
}

void Human::Error(const int& errNum)
{
	DisplayForPutting();
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
	}
}



Bot::Bot() : Player()
{}

void Bot::PutsShip()
{
	const std::vector<char> orientation = { RIGHT, LEFT, UP, DOWN };					// все направления
	std::vector<char> chooseOrientation;												// выбранные направления (возможные)
	std::vector<int> shipNumber; for (int i = 0; i < 4; i++) shipNumber.push_back(i);	// номер корабля, НЕ сам корабль
	std::vector<int> boardNum; for (int i = 0; i < 100; i++) boardNum.push_back(i);		// точки доски от 0 до 100

	srand(static_cast<unsigned int>(time(0)));
	int rNumShip = 0;								// рандомный номер корабля
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
		p_ship = GetShipAdress(rNumShip);

		// если было 3 попытки поставить корабль - удалить последний поставленный
		if (step == 3)
		{
			deck = m_board.DeleteLastShip(boardNum);
			CountPlus(deck - 1);
			if (GetCount(deck - 1) == 1) shipNumber.push_back(deck - 1);
			step = 0; cat2++;
		}

		rNumBoard = rand() % boardNum.size(); rNumBoard = boardNum[rNumBoard];

		// если не однопалубный - найти возможные направления
		if (p_ship->GetDeck() != 1)
		{
			step++;
			// найти направления, вмещающиеся в строку и/или не выходящие за границы доски
			if ((rNumBoard + (p_ship->GetDeck() - 1)) > (rNumBoard / BOARD_LEN) * BOARD_LEN + 9); else chooseOrientation.push_back(orientation[0]);
			if ((rNumBoard - (p_ship->GetDeck() - 1)) < (rNumBoard / BOARD_LEN) * BOARD_LEN); else chooseOrientation.push_back(orientation[1]);
			if ((rNumBoard - BOARD_LEN * (p_ship->GetDeck() - 1)) < 0); else chooseOrientation.push_back(orientation[2]);
			if ((rNumBoard + BOARD_LEN * (p_ship->GetDeck() - 1)) >= BOARD_LEN * BOARD_LEN); else chooseOrientation.push_back(orientation[3]);
			// найти направления из уже подходящих, которые НЕ пересекают границы др. кораблей
			for (int i = rNumBoard; i < rNumBoard + p_ship->GetDeck(); i++) if (m_board.GetSymbolPoint(i) == CLOSE_SYMBOL)
				for (int j = 0; j < chooseOrientation.size(); j++) if (chooseOrientation[j] == RIGHT) chooseOrientation.erase(chooseOrientation.begin() + j);
			for (int i = rNumBoard; i > rNumBoard - p_ship->GetDeck(); i--) if (m_board.GetSymbolPoint(i) == CLOSE_SYMBOL)
				for (int j = 0; j < chooseOrientation.size(); j++) if (chooseOrientation[j] == LEFT) chooseOrientation.erase(chooseOrientation.begin() + j);
			for (int i = rNumBoard; i > rNumBoard - p_ship->GetDeck() * BOARD_LEN; i -= BOARD_LEN) if (m_board.GetSymbolPoint(i) == CLOSE_SYMBOL)
				for (int j = 0; j < chooseOrientation.size(); j++) if (chooseOrientation[j] == UP) chooseOrientation.erase(chooseOrientation.begin() + j);
			for (int i = rNumBoard; i < rNumBoard + p_ship->GetDeck() * BOARD_LEN; i += BOARD_LEN) if (m_board.GetSymbolPoint(i) == CLOSE_SYMBOL)
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
		if (p_ship->GetDeck() != 1) input += chooseOrientation[rNumOrientation];
		m_board.PutShip(input, *p_ship);
		CountMinus(rNumShip);
		if (GetCount(rNumShip) == 0) for (int i = 0; i < static_cast<int>(shipNumber.size()); i++)
			if (shipNumber[i] == rNumShip) shipNumber.erase(shipNumber.begin() + i);

		input = "";
		chooseOrientation.clear();
	}
	//std::cout << "\n cat = " << cat << " cat2 = " << cat2 << std::endl;
	return;
}

void Bot::DelNumFromBoard(int point, char orientation, std::vector<int>& numbers, ship& obj, Player* const p)
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
			if (p != 0) if (p->GetStepFromBoard(leftUpPoint + l) != 1)
				p->SetStepOnBoard(leftUpPoint + l, 1);
		}
		leftUpPoint += 10;
	}
}