#pragma once
#include "HeaderAndStructures.h"
#include "UnitObjectClass.h"
#include "AnimationSetContainer.h"

class GameViewUnitContainer
{
public:
	GameViewUnitContainer();
	~GameViewUnitContainer();

	QMap<sf::Vector2i,UnitObjectClass*> UnitOnMapContainer;
	AnimationSetContainer Animation_Units_Objects;
	int Scale = 1;

	void DrawUnits(sf::RenderWindow &Window);

 void MapCellPressed(int x,int y);
};