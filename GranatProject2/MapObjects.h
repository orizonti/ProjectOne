#pragma once
#include "HeaderAndStructures.h"
#include "GameImageClass.h"
#include "GameCoord.h"
#include "AnimationSetContainer.h"

class MapContainerClass;

class MapObject
{
public:
	MapObject();
	MapObject(QString Type);
	static MapContainerClass *TerrainMap;
	double Elevation;
	QString Type;

	void SetPosition(int x,int y);
	sf::Vector2f GetCoord();
	std::shared_ptr<SimpleImage> ObjectImage;

	void SetElevationStep(double Step);
	double ElevationStep = 0;
	void LiftObject(double Step);

	GameCoord CurrentPosition;

	void DrawObject(sf::RenderWindow& Window);
};


class MapGroupObject :public MapObject
{
public:
	MapGroupObject();
	MapGroupObject(QString TypeObject,int size);
	int GroupSize = 0;

	void DrawObject(sf::RenderWindow& Window);
	void AppendObject(std::shared_ptr<MapObject>& Object);
	QVector<std::shared_ptr<MapObject>> MapObjects;
};

class MapLiveObject :public MapGroupObject
{
public:
	MapLiveObject();
	MapLiveObject(QString Type, int size);
};

class MapUnitObject :public MapLiveObject
{
public:
	MapUnitObject(QString Type,int Size = 4);
	~MapUnitObject();

	static QMap<QPair<int, int>, Direction> DirectionTable;

	bool FLAG_UNIT_AT_MOVING = false;
	QVector<GameCoord> RoutePoints;


	int d_x = 0;
	int d_y = 0;
	int dir_x = 0;
	int dir_y = 0;


	void GetMoveDirection();
	void SetDestination(int x,int y);
	void MoveUnit();

	void SetPosition(int x,int y);
	sf::Vector2f GetCoord();

	GameCoord Destination;
	GameCoord EndDestination;
	GameCoord NextCell;
};



