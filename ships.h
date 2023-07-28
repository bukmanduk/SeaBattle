#include "console.h"

class ship 
{
public:
	ship(int deck = 1);
	int GetDeck() const;
	bool ShipDestroyed();
	void Hit();
	~ship();
protected:
	int m_deck;
	int m_life;
};