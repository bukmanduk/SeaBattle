#include "ships.h"

const int BOARD_LEN = 10;

class board
{
public:
	board();

	char GetSymbolPoint(const int point) const;
	int GetStepBoard(const int point) const;
	ship* GetAdress(const int point) const;

	void SetSymbolPoint(const int point, const char symbol = SHIP_SYMBOL);
	void SetStepBoard(const int point, const int step);
	void SetShipPoint(const int point, ship& obj);
	void SetStep(const int step);

	void ClearAdress(const int point);
	void PutShip(const std::string &input, ship &obj);
	int DeleteLastShip();
	int DeleteLastShip(std::vector<int>& numbers);

	void DisplayBoard();
	void DisplayBoards();

	static int StringToPoint(const std::string& input);
	static std::string PointToString(const int point);

	~board();
private:
	ship* p_board[BOARD_LEN * BOARD_LEN];
	char  m_board[BOARD_LEN * BOARD_LEN];
	int   m_steps[BOARD_LEN * BOARD_LEN];
	int   m_step;
};
