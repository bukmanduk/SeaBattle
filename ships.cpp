#include "ships.h"

ship::ship(int deck) : m_deck(deck), m_life(deck)
{}

int ship::GetDeck() const { return m_deck; }

bool ship::ShipDestroyed() 
{ 
	if (m_life == 0) return true; 
	return false;
}

void ship::Hit()
{
	if (!ShipDestroyed()) m_life--;
}

ship::~ship()
{}