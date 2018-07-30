#pragma once
#include <HeaderAndStructures.h>
#include <GridShapeContainer.h>

class TerrainTileElement
{
public:
	TerrainTileElement();
	~TerrainTileElement();
	sf::Texture* Texture;
	sf::Sprite* Sprite;
	GridShapeContainer* GridLines = 0;
	int type_id;
	QString Name = "";
	QSize size;
	QPair<int, int> offset;
	QPair<int, int> offsetGrid;

};


class TileSetClass
{
public:
	TileSetClass();
	void CreateTileSetFromMap(QString MapFilePath);

	QMap<int,TerrainTileElement*> TerrainElementsByType;
	QMap<QString,TerrainTileElement*> TerrainElementsByName;


	void UploadTerrainElementData(QString PathXMLFile, int type_id);
	void UploadGridLines(QString PathXMLSprite, TerrainTileElement* element);


	~TileSetClass();
};

