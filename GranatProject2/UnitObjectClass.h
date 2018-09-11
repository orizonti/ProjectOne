#pragma once
#include "HeaderAndStructures.h"
#include "GameImageClass.h"
#include "GameCoord.h"
#include "AnimationSetContainer.h"

class MapContainerClass;

class UnitObjectClass
{
public:
	UnitObjectClass(QString Type);
	~UnitObjectClass();

	static MapContainerClass *TerrainMap;
	static QMap<QPair<int, int>, Direction> DirectionTable;

	bool FLAG_UNIT_AT_MOVING = false;


	int d_x = 0;
	int d_y = 0;
	int dir_x = 0;
	int dir_y = 0;

	QVector<double> Elevations;

	QVector<GameCoord> RoutePoints;


	QString TypeUnit;


	void SetPosition(int x,int y);
	void GetMoveDirection();
	void SetDestination(int x,int y);
	void MoveUnit();

	sf::Vector2f GetCoord();
	GroupImage UnitImage;

	GameCoord CurrentPosition;
	GameCoord Destination;
	GameCoord EndDestination;
	GameCoord NextCell;
};



