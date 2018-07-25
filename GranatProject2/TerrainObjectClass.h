#pragma once
#include "UnitObjectClass.h"
#include <GridShapeContainer.h>
#include "TileSetClass.h"
#include "HeaderAndStructures.h"



class TerrainObjectClass
{
public:
	TerrainObjectClass();
	TerrainTileElement* TerrainData = 0;

	void SetCoord(int x, int y);
private:
public:
	CoordClass Position;
	int TerrainType;
	QSize TileSize;
	sf::Sprite* GetSpriteToDraw();
};