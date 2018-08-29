#pragma once
#include "HeaderAndStructures.h"
#include "GameImageClass.h"
#include "GameCoord.h"
#include "AnimationSetContainer.h"


class UnitObjectClass
{
public:
	UnitObjectClass(QString Type);
	~UnitObjectClass();

	static std::shared_ptr<AnimationSetContainer> Animations;
	       std::shared_ptr<ClassAnimationSet>     UnitAnimation = NULL;

	QString TypeUnit;

	GameImage UnitImage;
	int CurrentFrame = 0;
	GameCoord CurrentPosition;
	GameCoord Destination;

	void SetPosition(int x,int y);
	void SetDestination(int x,int y);
	void MoveUnit();

	sf::Vector2i GetCoord();
};

class ClassWarriorUnit :
	public UnitObjectClass
{
public:
	ClassWarriorUnit(QString TypeUnit);  // Test comment

	void Attack();


};


