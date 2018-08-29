#pragma once
#include "HeaderAndStructures.h"

class GameCoord
{
public:
	~GameCoord();
	GameCoord();
	static QSize CellSize;

	Eigen::Matrix2d n;
	Eigen::Matrix2d m;


	Eigen::Vector2d MousePosReal;
	Eigen::Vector2d DecPos;
	Eigen::Vector2d IsoPos;


	void SetCoordDecart(int x, int y);
	void SetCoordIsometric(int x, int y);
	void SetRealCoord(double x, double y);

	void translate(int d_x, int d_y);

	QPair<int, int> GetIsoCoord();
	QPair<int, int> GetDecCoord();

	 sf::Vector2i GetIsoVector();
	 sf::Vector2f GetDecVector();

	bool operator==(GameCoord &Coord)
	{
		if (this->DecPos == Coord.DecPos)
			return true;
		else
			return false;

	}


	void operator=(GameCoord& Coord)
	{
		this->SetCoordIsometric(Coord.IsoPos(0), Coord.IsoPos(1));
	}

};

