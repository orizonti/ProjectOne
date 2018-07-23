#pragma once
#include "UnitObjectClass.h"
#include "SFML\Graphics.hpp"
#include <Eigen/Dense>
#include <iostream>
#include <qstring>
#include "QPolygon"
#include "qfile.h"
#include "QtXml\qxml.h"
#include "QtXml\qdom.h"
#include <GridShapeContainer.h>
#include "TileSetClass.h"


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