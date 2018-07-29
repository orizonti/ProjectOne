#pragma once

#include "TerrainObjectClass.h"
#include "TerrainObjectClass.h"
#include "GameViewUnitContainer.h"
#include "TileSetClass.h"

class MapContainerClass
{
public:
	MapContainerClass();
	~MapContainerClass()
	{
	}

	QSize MapSize;


	void CreateMapFromFile(QString MapFilePath = "E:/WorkDir/WORK_DIR/MAPS_TILED/TestMapBig.tmx");

	QMap<int,QVector<TerrainObjectClass*> > TerrainLayers;
	TileSetClass TileSet;

	void DrawTerrain(sf::RenderWindow &Window);

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
	Eigen::Vector2d MousePosReal;

    GameCoord MousePosition;

	 void KeyboardControl(sf::Event event);
	 void MouseControl(sf::Event event);

	 void DrawMap();
	 
	 MapContainerClass Map;
	 GameViewUnitContainer Units;
};
