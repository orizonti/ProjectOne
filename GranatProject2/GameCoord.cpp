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
void GameCoord::SetRealCoord(double x, double y)
{
	DecPos(0) = x;
	DecPos(1) = y;


				IsoPos = n*DecPos;
				MousePosReal = m*IsoPos*CellSize.height();

				IsoPos(0) = floor(IsoPos(0));
				IsoPos(1) = floor(IsoPos(1));

				DecPos = m*IsoPos * CellSize.height();
}

void GameCoord::SetCoordDecart(float x, float y)
{
	DecPos(0) = x;
	DecPos(1) = y;
	IsoPos = n*DecPos;
}

void GameCoord::SetCoordIsometric(double x, double y)
{
	IsoPos(0) = x;
	IsoPos(1) = y;

	DecPos = m*IsoPos * CellSize.height();
}

QPair<float, float> GameCoord::GetIsoCoord()
{
	QPair<float, float> Coord;
				Coord.first = IsoPos(0);
				Coord.second = IsoPos(1);
				return Coord;
}
QPair<float, float> GameCoord::GetDecCoord()
{
	QPair<float, float> Coord;
				Coord.first = DecPos(0);
				Coord.second = DecPos(1);
				return Coord;
}
sf::Vector2f GameCoord::GetIsoVector()
{
	sf::Vector2f Coord;
	Coord.x = IsoPos(0);
	Coord.y = IsoPos(1);
	return Coord;
}
sf::Vector2f GameCoord::GetDecVector()
{
	sf::Vector2f Coord;
	Coord.x = DecPos(0);
	Coord.y = DecPos(1);
	return Coord;
}

void GameCoord::translate(double d_x, double d_y)
{
	this->SetCoordIsometric(IsoPos(0) + d_x, IsoPos(1) + d_y);
}

void GameCoord::translateDecart(double d_x, double d_y)
{
	this->SetCoordDecart(DecPos(0) + d_x, DecPos(1) + d_y);
}

//void GameCoord::translate(int d_x, int d_y)
//{
//	float new_x_iso = IsoPos(0) + d_x;
//	float new_y_iso = IsoPos(1) + d_y;

//	this->SetCoordIsometric(new_x_iso, new_y_iso);
//}