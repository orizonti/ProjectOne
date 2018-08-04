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

		sf::Image img;

	sf::Texture texture;
	 
	sf::Sprite sprite;

	QSize MapSize;
	sf::Texture BorderCellTexture;
	sf::Sprite BorderCellSprite;
	GameCoord CursorPosition;
	GameCoord CursorPosition2;

	 bool FLAG_DRAW_GRID = false;

	void CreateMapFromFile(QString MapFilePath = "E:/WorkDir/WORK_DIR/MAPS_TILED/TestMapBig.tmx");

	QMap<int,Terrains> TerrainLayers;
	QMap<PairCoord, Terrains> ClusteredObjects;
	QMap<PairCoord, PairCoord> ClustersOffsetFromCenter;
	TileSetClass TileSet;

	void TerrainClasterization(QVector<TerrainObjectClass*> TerrainLayer);

	void DrawTerrain(sf::RenderWindow &Window);
	void DrawCurrentCell(sf::RenderWindow &Window);

	void MapCellPressed(int x, int y);
	void MapCellMoved(int x, int );
};

class MapDisplayEngine :public EventControlInterface
{
public: 
	MapDisplayEngine();
	~MapDisplayEngine();

	 sf::RenderWindow* Window;
	 sf::View* Camera;

	 float Scale = 1;
	 QSize WindowSize;
	 QSize CellSize;

	Eigen::Vector2d OffsetCamera;

    GameCoord MousePosition;

	 void KeyboardControl(sf::Event event);
	 void MouseControl(sf::Event event);

	 void DrawMap();
	 
	 MapContainerClass Map;
	 GameViewUnitContainer Units;
};
