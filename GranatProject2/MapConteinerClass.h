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
	~MapConteinerClass()
	{

	delete TerrainTypes.at(1);
	delete TerrainTypes.at(2);

	}

	QSize MapSize;
	void DrawMap(sf::RenderWindow &Window);

	std::vector<TerrainObjectClass> MapObjects;
	std::map<int,sf::Sprite*> TerrainTypes;
	QMap<int,sf::Sprite> TerrainTypes2;
	void CreateMapFromFile(QString MapFilePath = "E:/WorkDir/WORK_DIR/MAPS_TILED/TestMapBig.tmx");
	void UploadImage(std::string ImagesFile, int Type);

		QVector<MapItem> TerrainItems;
		QVector<MapItem> LandScapeItems;


	Eigen::Matrix2d m;
	Eigen::Matrix2d n;

	Eigen::Vector2d IsoVect;
	Eigen::Vector2d IsoVect2;
	Eigen::Vector2d DecVect;
	Eigen::Vector2d DecVect2;

	sf::Image   terrainImage;
	sf::Texture textureTerrain;
	sf::Sprite  spriteTerrain;
};

