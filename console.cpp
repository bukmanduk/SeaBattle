#include "console.h"

void Console::Pause()
{
	system("pause");
}

void Console::Clear()
{
	system("cls");
}

void Console::Read(std::string& input)
{
	std::cin >> input;
	std::cin.clear();
	std::cin.ignore((std::cin.rdbuf()->in_avail()));
}

void Console::QuitProgram()
{
	exit(0);
}