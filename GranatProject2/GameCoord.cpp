#include "GameCoord.h"



GameCoord::GameCoord()
{
		m << 1, 1,
			-0.5, 0.5;

		n << 0.5, -1,
			0.5, 1;

		IsoPos(0) = 0;
		IsoPos(1) = 0;
}


GameCoord::~GameCoord()
{
}

void GameCoord::SetCoordDecart(int x, int y)
{
	DecPos(0) = x;
	DecPos(1) = y;
	IsoPos = n*DecPos;
}

void GameCoord::SetCoordIsometric(int x, int y)
{
	IsoPos(0) = x - 1;
	IsoPos(1) = y - 1;

	DecPos = m*IsoPos * 64;

}
