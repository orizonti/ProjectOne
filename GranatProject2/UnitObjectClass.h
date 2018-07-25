#pragma once
#include "HeaderAndStructures.h"
#include "GameImageClass.h"
#include "GameCoord.h"


class UnitObjectClass
{
public:
	UnitObjectClass();
	~UnitObjectClass();
	GameImage UnitImage;

	GameCoord CurrentPosition;
	GameCoord Destination;
};

class ClassHero :
	public UnitObjectClass
{
public:
	ClassHero()  // Test comment
	{
		SetPosition(3, 4);
		SetDestination(3, 4);
	}

	int CurrentFrame = 0;

	void MoveHero();
	void SetPosition(int x,int y);
	void SetDestination(int x,int y);

};

