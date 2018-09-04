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
	GameCoord PositionTopRight;
	int TerrainType;
	QSize TileSize;
	sf::Sprite* GetSpriteToDraw();

	static int OFFSET_HEIGHT_TEXT;
	bool FLAG_MOUSE_MOVED = false;

	QVector<QPainterPath> CellPathes;
	QPainterPath PathContour;
	CurveShape ShapeContour;
	QVector<CurveShape> ShapesCell;
	int Number_Cell_Moved = 0;

	QuadeRangleShape& GetCellBorderMoved();
	QPair<int, int> GetCellPressed();

	bool CheckCursorPosition(int x, int y);
	bool ContainsMapPoint(int x, int y);

	QVector<double>& GetHeightMapOnCell(int x, int y);
	

	void DrawObject(sf::RenderWindow& Window);
	void DrawGrid(sf::RenderWindow& Window);
	void DrawContour(sf::RenderWindow& Window);

	void DrawTerrainHeight(sf::RenderWindow& Window);

};