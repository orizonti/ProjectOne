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
	GameCoord Position;
	int TerrainType;
	QSize TileSize;
	sf::Sprite* GetSpriteToDraw();

	void DrawObject(sf::RenderWindow& Window);
	void DrawGrid(sf::RenderWindow& Window);
};