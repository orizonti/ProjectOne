#pragma once
#include "HeaderAndStructures.h"

class GameCoord
{
public:
	~GameCoord();
	GameCoord();

	Eigen::Matrix2d n;
	Eigen::Matrix2d m;


	Eigen::Vector2d DecPos;
	Eigen::Vector2d IsoPos;

	void SetCoordDecart(int x, int y);
	void SetCoordIsometric(int x, int y);
	void SetRealCoord(int x, int y);


	bool operator==(GameCoord &Coord)
	{
		if (this->DecPos == Coord.DecPos)
			return true;
		else
			return false;

	}
};

