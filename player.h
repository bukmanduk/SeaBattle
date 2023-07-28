#include "board.h"

class Player
{
public:
	Player();

	char  GetSymbolFromBoard(const int point);
	int   GetStepFromBoard(const int point);
	ship* GetAdressFromBoard(const int point);
	ship* GetShipAdress(int number);
	int   GetCount(const int number) const;

	void SetSymbolOnBoard(const int point, char symbol = SHIP_SYMBOL);
	void SetStepOnBoard(const int point, const int step);
	void SetGeneralStep(const int point);

	bool EmptyShip() const;
	void CountMinus(const int number);
	void CountPlus(const int number);
	void ResetCount();

	virtual void PutsShip() = 0;
protected:
	std::vector<ship> m_ships;	 //нельзя изменять иначе потеряются ссылки
	board m_board;
	int   m_shipCount[4];		 // кол-во кораблей = {onedeck, twodeck, threedeck, fourdeck}
};

class Human : public Player
{
public:
	Human();
	void PutsShip() override;
private:
	bool IsInputCorrect(std::string& input, const ship& obj, const board& b);
	bool IsCorrectNum(std::string& input);
	void DisplayForPutting();
	void Error(const int& errNum);
};

class Bot : public Player
{
public:
	Bot();
	void PutsShip() override;
	static void DelNumFromBoard(int point, char orientation, std::vector<int>& numbers, ship& obj, Player* const p = 0);
};