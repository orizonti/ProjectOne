#pragma once
#include "UnitObjectClass.h"
#include <GridShapeContainer.h>
#include "TileSetClass.h"
#include "HeaderAndStructures.h"



class TerrainObjectClass
{
public:
	TerrainObjectClass(TerrainTileElement* Terrain);
	TerrainTileElement* TerrainData = 0;

	void SetCoord(int x, int y);
private:
public:
	GameCoord Position;
	int TerrainType;
	QSize TileSize;
	sf::Sprite* GetSpriteToDraw();

	bool FLAG_MOUSE_MOVED = false;

	QMap<int,QVector<QPainterPath>> StripesPath;
	QPainterPath PathContour;
	CurveShape ShapeContour;

	bool CheckCursorPosition(int x, int y);

	void DrawObject(sf::RenderWindow& Window);
	void DrawGrid(sf::RenderWindow& Window);
	void DrawContour(sf::RenderWindow& Window);
};