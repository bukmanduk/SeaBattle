#include "board.h"

board::board() : m_step(0)
{
	for (int point = 0; point < BOARD_LEN * BOARD_LEN; point++)
	{
		m_board[point] = START_SYMBOL;
		p_board[point] = 0;
		m_steps[point] = 0;
	}
}

char board::GetSymbolPoint(const int point) const
{
	return m_board[point];
}

int board::GetStepBoard(const int point) const
{
	return m_steps[point];
}

ship* board::GetAdress(const int point) const
{
	return p_board[point];
}

void board::SetSymbolPoint(int point, char symbol)
{
	m_board[point] = symbol;
	m_steps[point] = m_step;
}

void board::SetStepBoard(const int point, const int step)
{
	m_steps[point] = step;
}

void board::SetShipPoint(const int point, ship& obj)
{
	p_board[point] = &obj;
	m_steps[point] = m_step;
}

void board::SetStep(const int step)
{
	m_step = step;
}

void board::ClearAdress(const int point)
{
	p_board[point] = 0;
}

void board::PutShip(const std::string &input, ship &obj)
{
	m_step++;
	int point = StringToPoint(input), leftUpPoint, lenght = 0, weight = 0;
	char orientation = RIGHT;
	if (obj.GetDeck() != 1) orientation = input[2];

	if (orientation == LEFT) point -= obj.GetDeck() - 1;
	else if (orientation == UP) point -= (obj.GetDeck() - 1) * 10;

	if (orientation == RIGHT || orientation == LEFT)
		for (int i = point; i < point + obj.GetDeck(); i++)
		{
			SetSymbolPoint(i);
			SetShipPoint(i, obj);
		}
	if (orientation == UP || orientation == DOWN)
		for (int i = point; i < point + obj.GetDeck() * BOARD_LEN; i += BOARD_LEN)
		{
			SetSymbolPoint(i);
			SetShipPoint(i, obj);
		}

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

	leftUpPoint = point;
	if (point > 9) leftUpPoint -= 10;
	if (point % 10 > 0) leftUpPoint -= 1;

	for (int w = 0; w < weight; w++)
	{
		for (int l = 0; l < lenght; l++)
		{
			if (GetSymbolPoint(leftUpPoint + l) == '+') SetSymbolPoint(leftUpPoint + l, '-');
		}
		leftUpPoint += 10;
	}
}

int board::DeleteLastShip()
{
	int deck = 0;
	if (m_step != 0) for (int point = 0; point < BOARD_LEN * BOARD_LEN; point++)
			if (GetStepBoard(point) == m_step)
			{
				SetSymbolPoint(point, START_SYMBOL);
				if (GetAdress(point) != 0 && deck == 0) deck = GetAdress(point)->GetDeck();
				ClearAdress(point);
				SetStepBoard(point, 0);
			}
	m_step--;
	return deck;
}

int board::DeleteLastShip(std::vector<int>& numbers)
{
	int deck = 0;
	if (m_step != 0) for (int point = 0; point < BOARD_LEN * BOARD_LEN; point++)
		if (GetStepBoard(point) == m_step)
		{
			SetSymbolPoint(point, START_SYMBOL);
			if (GetAdress(point) != 0 && deck == 0) deck = p_board[point]->GetDeck();
			ClearAdress(point);
			SetStepBoard(point, 0);
			numbers.push_back(point);
		}
	m_step--;
	return deck;
}

void board::DisplayBoard()
{
	std::cout << "\n        - YOUR BOARD -  \n";
	std::cout << "\n     A B C D E F G H I J\n";
	for (int i = 0; i < BOARD_LEN; i++)
	{
		std::cout << " " << std::setw(2) << std::right << i << " ";
		for (int j = 0; j < BOARD_LEN; j++) std::cout << " " << m_board[i * BOARD_LEN + j];
		std::cout << std::endl;
	}
}

void board::DisplayBoards()
{
	std::cout << "\n     A B C D E F G H I J         A        B        C        D        E        F        G        H        I        J\n";
	for (int i = 0; i < BOARD_LEN; i++)
	{
		std::cout << " " << std::setw(2) << std::right << i << " ";
		for (int j = 0; j < BOARD_LEN; j++) std::cout << " " << m_board[i * BOARD_LEN + j];
		std::cout << "  " << std::setw(2) << std::right << i << " ";
		for (int j = 0; j < BOARD_LEN; j++) std::cout << " " << p_board[i * BOARD_LEN + j];
		std::cout << "  " << std::setw(2) << std::right << i << " ";
		for (int j = 0; j < BOARD_LEN; j++) std::cout << " " << m_steps[i * BOARD_LEN + j];
		std::cout << std::endl;
	}
}

int board::StringToPoint(const std::string& input)
{
	int point = (input[0] - 48) * BOARD_LEN;
	int colNum = input[1] - 97; if (colNum < 0) colNum += 32;
	return point + colNum;
}

std::string board::PointToString(const int point)
{
	std::string input;
	input += char(point / 10 + 48);
	input += char(point % 10 + 65);
	return input;
}

board::~board()
{}