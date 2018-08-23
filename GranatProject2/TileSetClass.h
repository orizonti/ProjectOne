#pragma once
#include <HeaderAndStructures.h>
#include <GridShapeContainer.h>

class TerrainTileElement
{
public:
	TerrainTileElement();
	~TerrainTileElement();

	sf::Texture* Texture;//TEXTURE AND SPRITE TO DRAW TILE ON MAP
	sf::Sprite*  Sprite ;

	int     type_id;
	QString Name = "";
	QSize   size;

	QPair<int, int> offset;//OFFSET OF TILE IN IMAGE TO CUNJUGATION OF CONTIGOUS TILES
	QPair<int, int> offsetGrid;

	GridShapeContainer* GridLines = 0;
};


class TileSetClass
{
public:
	TileSetClass();
	void CreateTileSetFromMap(QString MapFilePath);

	QMap<int,TerrainTileElement*>     TerrainElementsByType;
	QMap<QString,TerrainTileElement*> TerrainElementsByName;


	void UploadTerrainElementData(QString PathXMLFile, int type_id);
	void UploadGridLines(QString PathXMLSprite, TerrainTileElement* element);


	~TileSetClass();
};

