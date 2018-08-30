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
	std::shared_ptr<ClassAnimationSet> UnitAnimation = NULL;

	QString TypeUnit;

	int CurrentFrame = 0;

	void SetPosition(int x,int y);
	void CheckHeightMap(int x, int y);
	void SetDestination(int x,int y);
	void MoveUnit();

	sf::Vector2i GetCoord();
	GameImage UnitImage;
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


