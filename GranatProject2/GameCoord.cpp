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
void GameCoord::SetRealCoord(int x, int y)
{
	DecPos(0) = x;
	DecPos(1) = y;

				IsoPos = n*DecPos;

				IsoPos(0) = floor(IsoPos(0));
				IsoPos(1) = floor(IsoPos(1));

				DecPos = m*IsoPos * 64;
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
