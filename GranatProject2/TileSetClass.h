#pragma once
#include <HeaderAndStructures.h>
#include <GridShapeContainer.h>

class TerrainTileElement
{
public:
	sf::Sprite* Sprite;
	GridShapeContainer* GridLines;
	int type_id;
	QSize size;
	QPair<int, int> offset;
	QPair<int, int> offsetGrid;

};


class TileSetClass
{
public:
	TileSetClass();
	void CreateTileSetFromMap(QString MapFilePath);

	std::map<int,TerrainTileElement*> TerrainElementsByType;
	std::map<QString,TerrainTileElement*> TerrainElementsByName;


	void UploadTerrainElementData(QString PathXMLFile, int type_id);
	void UploadGridLines(QString PathXMLSprite, TerrainTileElement* element);


	~TileSetClass();
};

