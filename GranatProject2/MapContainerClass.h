#pragma once

#include "TerrainObjectClass.h"
#include "TerrainObjectClass.h"
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

	QMap<int,QVector<TerrainObjectClass> > TerrainLayers;
	TileSetClass TileSet;


};

class MapDisplayEngine : EventControlInterface
{
public: 
	 sf::RenderWindow Window;
	 sf::View Camera;
	 float Scale = 1;
	 QSize WindowSize;

	Eigen::Vector2d OffsetCamera;
	Eigen::Vector2d MousePosReal;

    GameCoord MousePosition;

	 void KeyboardControl(sf::Event event);
	 void MouseControl(sf::Event event);

	 void DrawMap(sf::RenderWindow &Window);
	 
	 MapContainerClass Map;
};
