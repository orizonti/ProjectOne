#pragma once
#include "HeaderAndStructures.h"

class GameCoord
{
public:
	~GameCoord();
	GameCoord();
	GameCoord(const GameCoord& Coord)
	{
		this->SetCoordIsometric(Coord.IsoPos(0), Coord.IsoPos(1));
	}
	static QSize CellSize;

	Eigen::Matrix2d n;
	Eigen::Matrix2d m;


	Eigen::Vector2d MousePosReal;
	Eigen::Vector2d DecPos;
	Eigen::Vector2d IsoPos;


	void SetCoordDecart(float x, float y);
	void SetCoordIsometric(float x, float y);
	void SetRealCoord(double x, double y);

	void translate(float d_x, float d_y);

	QPair<float, float> GetIsoCoord();
	QPair<float, float> GetDecCoord();

	 sf::Vector2f GetIsoVector();
	 sf::Vector2f GetDecVector();

	bool operator==(GameCoord &Coord)
	{

		double d_x = this->IsoPos(0) - Coord.IsoPos(0);
		double d_y = this->IsoPos(1) - Coord.IsoPos(1);

		if (abs(d_x) < 0.0001 && abs(d_y) < 0.0001)
			return true;
		else
			return false;

	}

	bool operator!=(GameCoord &Coord)
	{
		return !(*this == Coord);
	}

//	void operator=(GameCoord& Coord)
//	{
//		this->SetCoordIsometric(Coord.IsoPos(0), Coord.IsoPos(1));
//	}

};

