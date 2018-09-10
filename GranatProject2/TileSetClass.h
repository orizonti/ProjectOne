#pragma once
#include <HeaderAndStructures.h>
#include <GridShapeContainer.h>
#include <GameCoord.h>

class TerrainTileElement
{
public:
	TerrainTileElement();
	~TerrainTileElement();

	void GetHeightMapToDraw(); // IT IS TEMPORARY DEBUGING METHOD
	void UploadData(QString PathXMLFile, int element_number); // UPLOADS SIZE, NAME, TEXTURE, SVG GRID LINES TO TILE ELEMENT
	void UploadGridData(QString PathXMLFile);          // UPLOADS SVG GRIDS, THAT IS USED TO DISPLAY GRIDS AND DEFINCE CELL PRESSING ON CURVED CELL

	void UploadHeightMap(QFile& XMLFileHeights);       // UPLOADS HEIGHTS ON TERRAIN CELLS TO DISPLAY UNITS ON CURVED CELL PROPERLY


	sf::Texture* Texture;//TEXTURE AND SPRITE TO DRAW TILE ON MAP
	sf::Sprite*  Sprite ;

	int     type_id;
	QString Name = "";
	QSize   Size_In_Pixels;
	QSize   Size_By_Cell;

	QPair<int, int> offset;//OFFSET OF TILE IN IMAGE TO CUNJUGATION OF CONTIGOUS TILES
	QPair<int, int> offsetGrid;

	GridShapeContainer* GridLines = 0;
	QVector<QVector<double>>  HeightMap;
	QVector<Draw_Height_Node> HeightMapToDraw;

};


class TileSetClass
{
public:
	TileSetClass();
	void CreateTileSetFromMap(QString MapFilePath);

	QMap<int,TerrainTileElement*>     TerrainElementsByType;
	QMap<QString,TerrainTileElement*> TerrainElementsByName;


	~TileSetClass();
};

