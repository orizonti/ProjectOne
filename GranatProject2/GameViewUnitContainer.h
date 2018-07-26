#pragma once
#include "HeaderAndStructures.h"
#include "UnitObjectClass.h"

class GameViewUnitContainer
{
	GameViewUnitContainer();
	~GameViewUnitContainer();

	QMap<sf::Vector2i,UnitObjectClass*> UnitOnMapContainer;
	int Scale = 1;

	void DrawUnits(sf::RenderWindow &Window);

 void MapCellPressed(int x,int y);
};