#include <iostream>
#include <vector>
#include <iomanip>
#include <string>

#define START_SYMBOL '+'
#define SHIP_SYMBOL 'S'
#define CLOSE_SYMBOL '-'
#define GAME_SYMBOL '.'
#define RIGHT '>'
#define LEFT '<'
#define UP '^'
#define DOWN 'v'
#define MISS 'X'
#define HIT 'V'

class Console
{
public:
	static void Pause();
	static void Clear();
	static void Read(std::string& input);
	static void QuitProgram();
};