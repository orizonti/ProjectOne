#pragma once
#include "HeaderAndStructures.h"
#include "MapObjects.h"
#include "AnimationSetContainer.h"

class GameViewUnitContainer
{
public:
	GameViewUnitContainer();
	~GameViewUnitContainer();
	std::shared_ptr<MapUnitObject> CurrentUnit = NULL;
	QMap<QPair<int,int>,std::shared_ptr<MapUnitObject>> UnitOnMapContainer;

	//int Scale = 1;

	void DrawUnits(sf::RenderWindow &Window);

	void MoveUnits();


 void MapCellPressed(int x,int y);
};