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


	bool FLAG_CURSOR_ON_HILL = false;
	bool FLAG_DRAW_GRID      = false;

	QSize MapSize;

	GameCoord CursorPosition; 
	GameCoord PressedPosition; 

	PairCoord           CurrentCenterCluster;//COORD POINT TO CURRENT TERRAIN IN CLUSTERED HILL OBJECTS USING AT DEFINE CELL METHOD AND WHEN DRAWING CLUSTER BORDER CONVEX
	TerrainObjectClass* CurrentTerrain = 0;  //IS NEEDED TO DRAW CURRENT CELL IN TERRAIN UNDER CURSOR

	//CREATING TERRAIN OBJECTS AND SORTING THEM BY GROUPS GROUND AND HILLS
	void CreateMapFromFile(QString MapFilePath = "E:/WorkDir/WORK_DIR/MAPS_TILED/TestMapBig.tmx");

	QuadeRangleShape BorderCell                      ;
	QuadeRangleShape BorderCellPressed               ;
	QMap<int,Terrains>              TerrainLayers    ;//TERRAIN OBJECTS DEVIDED TO LAYERS, GROUNDS and HILLS 
	QMap<PairCoord,sf::ConvexShape> ConvexToClusters ;//QUADERANGLE OF HILL CLUSTER TO DRAW RED BORDER AROUND HILLS WHEN CURSOR MOVING ON THEM
	QVector<CurveShape>             PlainGridLines   ;//BLACK LINES THAT IS DRAWED ON ALL MAP UNDER HILLS LAYER

	QMap<PairCoord, Terrains>       ClusteredObjects ;//OBJECTS IS WANTED TO DEFINE CURRENT TERRAIN AND CELL IN HILL 
	QMap<PairCoord,QPainterPath>    PathToClusters   ;//PAINTER PATH QUADRANGLE TO DEFINE THAT CURSOR POSITION IS IN HILL BOUND
	
	TileSetClass TileSet; //CONTAINER WITH TERRAIN ELEMENT OBJECTS THAT HAS IMAGE, GRID SHAPE BUT NOT ATTACHED TO ANY COORD IN MAP, IT IS IN TERRAIN_OBJECT_CLASS

	void DrawTerrain(sf::RenderWindow &Window);
	void DrawCurrentCell(sf::RenderWindow &Window);

	void MapCellPressed(int x, int y);
	void MapCellMoved(int x, int );
	void DefineCellMoved(int x,int y); //DEFINE THAT CURSOR IS MOVING UNDER ANY HILL TO DRAW CELL BORDER ON HILL
	QPair<int, int> GetRealCellPressed();
	QVector<double>& GetCellHeightMap(int x, int y);

				private:
				//UTILITY METHODS TO DEFINE CURRENT HILL UNDER CURSOR
				PairCoord CalculateNearestCluster(int x, int y);
				PairCoord DefineBelongPoint(PairCoord NearestCenter, GameCoord& Coord);//DEFINE IF CURRENT COORD IS CONTAINED IN HILL'S BOUND USING PATH_TO_CLUSTERS MAP 
				void TerrainClasterization(QVector<TerrainObjectClass*> TerrainLayer);//TERRAIN CLUSTERIZATION TO LOCATE TERRAIN CURRENT HILL UNDER CURSOR

};

