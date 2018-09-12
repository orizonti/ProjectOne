#pragma once
#include "MapObjects.h"
#include <GridShapeContainer.h>
#include "TileSetClass.h"
#include "HeaderAndStructures.h"



class TerrainObjectClass
{
public:
	TerrainObjectClass(int Type);
	TerrainTileElement* TerrainData = 0;   //TERRAIN OBJECT CONTAINS COMMON TERRAIN_TILE_ELEMENT

	void SetCoord(int x, int y);

	static void LoadTileSet();
	QuadeRangleShape& GetCellBorderMoved(); // GET QUADE CONTOUR TO HIGHLIGHT CURRENT CELL ON CURRENT TERRAIN OBJECT
	QPair<int, int>   GetCellPressed();
	//---------------------------------------------------
	bool CheckCursorPosition(int x, int y);
	bool ContainsMapPoint(int x, int y);

	//---------------------------------------------------
	QVector<double>& GetHeightMapOnCell(int x, int y); // USED WHEN UNIT IS MOVING ON MAP AND GETTING CELL HEIGHTS TO DRAWING UNIT PROPERLY

	void DrawObject(sf::RenderWindow& Window);
	void DrawGrid(sf::RenderWindow& Window);
	void DrawTerrainHeight(sf::RenderWindow& Window); //METHOD IS USED TO DEBUGING, SHOULD BE DELETED

	//---------------------------------------------------
	int TerrainType;
	QSize TileSize;
	bool FLAG_MOUSE_MOVED = false;
	GameCoord Position;
	//---------------------------------------------------

private:
	GameCoord PositionTopRight;
	//QVector<CurveShape> ShapesCell;

	QVector<QPainterPath> CellPathes;
	        QPainterPath  PathContour;

	static std::shared_ptr<TileSetClass> TileSet;

	int Number_Cell_Moved = 0;
	sf::Sprite* GetSpriteToDraw();
	//void DrawContour(sf::RenderWindow& Window);       //METHOD IS USED TO DEBUGING, SHOULD BE DELETED

};