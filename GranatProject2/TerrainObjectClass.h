#pragma once
#include "UnitObjectClass.h"


class TerrainObjectClass
{
public:
	sf::Image* Terrain = 0;
	sf::Texture* TerrainTexture = 0;
	sf::Sprite* TerrainSprite = 0;

	CoordClass Position;
	int TerrainType;
private:
public:
	QSize TileSize;
};