#pragma once
#include "HeaderAndStructures.h"
#include "UnitObjectClass.h"
#include "AnimationSetContainer.h"

class GameViewUnitContainer
{
public:
	GameViewUnitContainer();
	~GameViewUnitContainer();

	QMap<QPair<int,int>,std::shared_ptr<UnitObjectClass>> UnitOnMapContainer;

	std::shared_ptr<AnimationSetContainer> Animation_Units_Objects;
	int Scale = 1;

	void DrawUnits(sf::RenderWindow &Window);

 void MapCellPressed(int x,int y);
};