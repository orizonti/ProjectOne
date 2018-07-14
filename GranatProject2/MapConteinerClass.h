#pragma once
#include <QSize>

#include "SFML\Graphics.hpp"
#include "TerrainObjectClass.h"
#include "qfile.h"
#include "QtXml\qxml.h"
#include "QtXml\qdom.h"

struct MapItem
{
	std::pair<int, int> coord;
	int type;
};

class MapConteinerClass
{
public:
	QSize MapSize;
	void DrawMap(sf::RenderWindow &Window);

	std::vector<TerrainObjectClass> MapObjects;
	std::map<sf::Texture, int> TerrainTypes;
	void CreateMapFromFile(QString MapFilePath = "E:/WorkDir/WORK_DIR/MAPS_TILED/TestMapBig.tmx");
};