#pragma once

#include "TerrainObjectClass.h"
#include "TerrainObjectClass.h"
#include "TileSetClass.h"

class MapConteinerClass
{
public:
	MapConteinerClass();
	~MapConteinerClass()
	{
	}

	QSize MapSize;
	void DrawMap(sf::RenderWindow &Window);
	void CreateMapFromFile(QString MapFilePath = "E:/WorkDir/WORK_DIR/MAPS_TILED/TestMapBig.tmx");

	QMap<int,QVector<TerrainObjectClass> > TerrainLayers;
	TileSetClass TileSet;

};

