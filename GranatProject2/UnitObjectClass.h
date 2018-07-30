#pragma once
#include "HeaderAndStructures.h"
#include "GameImageClass.h"
#include "GameCoord.h"


class UnitObjectClass
{
public:
	UnitObjectClass(QString TypeUnit);
	~UnitObjectClass();
	GameImage UnitImage;

	GameCoord CurrentPosition;
	GameCoord Destination;
};

class ClassHero :
	public UnitObjectClass
{
public:
	ClassHero(QString TypeUnit);  // Test comment

	int CurrentFrame = 0;

	void MoveHero();
	void SetPosition(int x,int y);
	void SetDestination(int x,int y);

};

