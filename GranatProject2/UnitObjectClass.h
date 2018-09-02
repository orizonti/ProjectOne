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

	static std::shared_ptr<AnimationSetContainer> Animations;
	std::shared_ptr<AnimationSet> UnitAnimation = NULL;

	bool FLAG_UNIT_AT_MOVING = false;

	int d_x = 0;
	int d_y = 0;

	QVector<GameCoord> RoutePoints;

	QString TypeUnit;

	int CurrentFrame = 0;

	void SetPosition(int x,int y);
	void CheckHeightMap(int x, int y);
	void SetDestination(int x,int y);
	void MoveUnit();

	sf::Vector2f GetCoord();
	GroupImage UnitImage;
	GameCoord CurrentPosition;
	GameCoord Destination;
	static MapContainerClass *TerrainMap;
};

class ClassWarriorUnit :
	public UnitObjectClass
{
public:
	ClassWarriorUnit(QString TypeUnit);  // Test comment

	void Attack();


};


