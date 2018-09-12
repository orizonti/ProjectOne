#pragma once

#include "TerrainObjectClass.h"
#include "TerrainObjectClass.h"
#include "GameViewUnitContainer.h"
#include "TileSetClass.h"

typedef QVector<TerrainObjectClass*> Terrains;
typedef QPair<int, int> PairCoord;

class MapContainerClass
{
public:
	MapContainerClass();
	~MapContainerClass()
	{	
	}
	//---------------------------------------------------------------

	QPair<int, int>  GetRealCellPressed();
	QVector<double> GetCellHeightMap(int x, int y); //METHOD IS USED BY UNIT OBJECT WHEN UNIT IS MOVING ON CELL TO DRAW SPRITE ELEVATION PROPERLY

	TerrainObjectClass* CurrentTerrain = 0;  //IS NEEDED TO DRAW CURRENT CELL IN TERRAIN UNDER CURSOR


	void DefineCellMoved(int x,int y); //DEFINE THAT CURSOR IS MOVING UNDER ANY HILL TO DRAW CELL BORDER ON HILL
	void MapCellPressed(int x, int y);
	void MapCellMoved(int x, int );

	void DrawTerrain(sf::RenderWindow &Window);
	void DrawCurrentCell(sf::RenderWindow &Window);
	//---------------------------------------------------------------

	//================================================================
				private:
	//CREATING TERRAIN OBJECTS AND SORTING THEM BY GROUPS GROUND AND HILLS
	void CreateMapFromFile(QString MapFilePath = "E:/WorkDir/WORK_DIR/MAPS_TILED/TestMapBig.tmx");

				//UTILITY METHODS TO DEFINE CURRENT HILL UNDER CURSOR
				PairCoord CalculateNearestCluster(int x, int y);
				PairCoord DefineBelongPoint(PairCoord NearestCenter, GameCoord& Coord);//DEFINE IF CURRENT COORD IS CONTAINED IN HILL'S BOUND USING PATH_TO_CLUSTERS MAP 
				void TerrainClasterization(QVector<TerrainObjectClass*> TerrainLayer);//TERRAIN CLUSTERIZATION TO LOCATE TERRAIN CURRENT HILL UNDER CURSOR


	//------------------------------------------------
	QuadeRangleShape BorderCell                      ; //BORDER CELL WITH RED COLOR, THAT IS DRAWING WHEN MOUSE MOVING UNDER CELL  
	QuadeRangleShape BorderCellPressed               ; //BORDER CELL WITH YELLOW COLOR, THAT IS DRAWING WHEN CELL HAS BEEN PRESSED BY MOUSE
	//------------------------------------------------

	QMap<int,Terrains>  TerrainLayers    ;//TERRAIN OBJECTS DEVIDED TO LAYERS, GROUNDS and HILLS 
	QVector<CurveShape> PlainGridLines   ;//BLACK LINES THAT IS DRAWED ON ALL MAP UNDER HILLS LAYER

	//------------------------------------------------
	QMap<PairCoord, Terrains>       ClusteredObjects ;//OBJECTS IS WANTED TO DEFINE CURRENT TERRAIN AND CELL IN HILL 
	QMap<PairCoord,QPainterPath>    PathToClusters   ;//PAINTER PATH QUADRANGLE TO DEFINE THAT CURSOR POSITION IS IN HILL BOUND
	//------------------------------------------------

	QMap<PairCoord,sf::ConvexShape> ConvexToClusters ;//QUADERANGLE OF HILL CLUSTER TO DRAW RED BORDER AROUND HILLS WHEN CURSOR MOVING ON THEM

	PairCoord CurrentCenterCluster;//COORD POINT TO CURRENT TERRAIN IN CLUSTERED HILL OBJECTS USING AT DEFINE CELL METHOD AND WHEN DRAWING CLUSTER BORDER CONVEX
	GameCoord CursorPosition; 
	GameCoord PressedPosition; 

	bool FLAG_CURSOR_ON_HILL = false;

    //TileSetClass TileSet; //CONTAINER WITH TERRAIN ELEMENT OBJECTS THAT HAS IMAGE, GRID SHAPE BUT NOT ATTACHED TO ANY COORD IN MAP, IT IS IN TERRAIN_OBJECT_CLASS
	//================================================================
};

