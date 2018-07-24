#pragma once
#include <QSize>

#include "SFML\Graphics.hpp"
#include "TerrainObjectClass.h"
#include "qfile.h"
#include "QtXml\qxml.h"
#include "QtXml\qdom.h"
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
	void DrawMap(sf::RenderWindow &Window);
	void CreateMapFromFile(QString MapFilePath = "E:/WorkDir/WORK_DIR/MAPS_TILED/TestMapBig.tmx");

	QMap<int,QVector<TerrainObjectClass> > TerrainLayers;
	TileSetClass TileSet;

};

