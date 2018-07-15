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
	MapConteinerClass();

	QSize MapSize;
	void DrawMap(sf::RenderWindow &Window);

	std::vector<TerrainObjectClass> MapObjects;
	std::map<int,sf::Sprite> TerrainTypes;
	void CreateMapFromFile(QString MapFilePath = "E:/WorkDir/WORK_DIR/MAPS_TILED/TestMapBig.tmx");
	void UploadImage(std::string ImagesFile, int Type);

		QVector<MapItem> TerrainItems;
		QVector<MapItem> LandScapeItems;

	sf::Image   terrainImage;
	sf::Texture textureTerrain;
	sf::Sprite  spriteTerrain;
};

