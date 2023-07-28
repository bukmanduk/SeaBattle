#include "player.h"

class SeaBattle
{
public:
	SeaBattle();
	void Start();
private:
	void PreStart();
	void RemakeBoardsForStart();

	void HumanMove();
	bool CellCorrect(std::string& input);

	void BotMove();
	void FindBadOrientations(int point, int step, std::vector<char>& orientation);
	bool GoodPoint(int point);

	void End();
	void DisplayRules();
	void DisplayBoards();
	void DisplayMoving(bool human = false);
	void Error(int errNum);
private:
	Human m_human;
	Bot   m_bot;
};