#pragma once
#include "HeaderAndStructures.h"
#include "GameImageClass.h"
#include "GameCoord.h"
#include "AnimationSetContainer.h"

class MapContainerClass;

struct MoveDirectionVector
{
	double Step_Iso_X = 0;
	double Step_Iso_Y = 0;
	double Step_Elevation = 0;

	double Speed = 0.025;

	void SetMoveVector(int Dir_X, int Dir_Y, int Elevation = 0)
	{
		Step_Elevation = Elevation;

	Unit_X = Dir_X;
	Unit_Y = Dir_Y;

	 Step_Iso_X = Unit_X * Speed;
	 Step_Iso_Y = Unit_Y * Speed;
	}

	bool IsNull()
	{
		if ((Unit_X + Unit_Y) == 0)
			return true;
		else
			return false;
	}

	int Unit_X = 0;
	int Unit_Y = 0;
};

class MapObject
{
public:
	MapObject();
	MapObject(QString Type);
	static MapContainerClass *TerrainMap;
	QString Type;

	void SetPosition(int x,int y);
	sf::Vector2f GetCoord();
	std::shared_ptr<SimpleImage> ObjectImage;

	MoveDirectionVector MoveDirection;

	GameCoord CurrentPosition;

	void MoveObjectOnStep();
	void MoveObjectOnStep(double Iso_Step_X, double Iso_Step_Y, double Lift_Step);
	void SetMoveDirection(double Iso_Step_X, double Iso_Step_Y, double Lift_Step);

	void LiftObject(double ElevationStep);
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



	void MoveObjectOnStep();
    void MoveObjectOnStep(double Iso_Step_X, double Iso_Step_Y, double Lift_Step);

	void ChangeMoveDirection();
	void SetDestination(int x,int y);
	void MoveUnit();

	void SetPosition(int x,int y);
	sf::Vector2f GetCoord();

	GameCoord Destination;
	GameCoord EndDestination;
	GameCoord NextCell;
};



