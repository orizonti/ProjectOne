#include "TileSetClass.h"


TerrainTileElement::TerrainTileElement()
{

}
TerrainTileElement::~TerrainTileElement()
{
	qDebug() << "Delete tile element";
	delete Texture;
	delete Sprite;
}

TileSetClass::TileSetClass()
{
}


TileSetClass::~TileSetClass()
{
}

void TileSetClass::CreateTileSetFromMap(QString MapFilePath)
{
		qDebug() << "CREATE TILE SET FROM MAP - " << MapFilePath;
		QFile XMLMapFile(MapFilePath);
		bool result = XMLMapFile.open(QIODevice::ReadOnly);

		QDomDocument newDomDoc;
		result = newDomDoc.setContent(&XMLMapFile);
		if (result)
		{
			QDomNode currentNod = newDomDoc.documentElement().firstChild();
			while (!currentNod.isNull())
			{
				QDomElement newElement = currentNod.toElement();

				qDebug() << "*************************************";

				if (newElement.tagName() == "tileset")
				{
					
					int type_id = newElement.attribute("firstgid").toInt();
					QString MapTilePath = newElement.attribute("source");
					UploadTerrainElementData(MapTilePath, type_id);
				}

				currentNod = currentNod.nextSibling();
			}
		}
	qDebug() << "Terrain types - " <<	TerrainElementsByType.size();

}


void TileSetClass::UploadTerrainElementData(QString PathXMLFile, int type_id)
{
							QFile XMLMapFile(GameDir + "/WORK_DIR/MAPS_TILED/" + PathXMLFile);
						    bool result = XMLMapFile.open(QIODevice::ReadOnly);

							TerrainTileElement* newTileElement = new TerrainTileElement;

							QDomDocument newDomDoc;
							result = newDomDoc.setContent(&XMLMapFile);

							QDomElement tileset = newDomDoc.elementsByTagName("tileset").at(0).toElement();
							QDomElement tileoffset = newDomDoc.elementsByTagName("tileoffset").at(0).toElement();
							QDomElement image = newDomDoc.elementsByTagName("image").at(0).toElement();

							QString name = tileset.attribute("name");

							newTileElement->size.setWidth(tileset.attribute("tilewidth").toInt()); 
							newTileElement->size.setHeight(tileset.attribute("tileheight").toInt());

							newTileElement->offset.first = tileoffset.attribute("x").toInt(); 
							newTileElement->offset.second = tileoffset.attribute("y").toInt();

							qDebug() << "LOAD TILE - " << name  << "ID - " << type_id << "OFFSET - " << newTileElement->offset;

							QString imageSource = image.attribute("source");
							imageSource.remove(0, 2);
							imageSource = GameDir + "/WORK_DIR" + imageSource;

							qDebug() << "UPLOAD TERRAIN - " << imageSource;

									sf::Texture* textureTerrain = new sf::Texture;
									sf::Sprite*  spriteTerrain = new sf::Sprite;

									result = textureTerrain->loadFromFile(imageSource.toStdString());
											 spriteTerrain->setTexture(*textureTerrain);

											 newTileElement->Texture = textureTerrain;
											 newTileElement->Sprite = spriteTerrain;
											 newTileElement->type_id = type_id;
											 newTileElement->Name = name;


											 UploadGridLines(PathXMLFile, newTileElement);

										TerrainElementsByType.insert(type_id, newTileElement);
										TerrainElementsByName.insert(name, newTileElement);

}

void TileSetClass::UploadGridLines(QString PathXMLSprite,TerrainTileElement* element)
{
	QString FileName = PathXMLSprite.split(".").first();
	QString PathTSX = QString("%1.tsx").arg(FileName);
	QString PathXMLLines = PathTSX.replace(".tsx", ".svg");

							QFile XMLGridFile(PathTSX);
						    bool result = XMLGridFile.open(QIODevice::ReadOnly);

							TerrainTileElement* newTileElement = new TerrainTileElement;

							QDomDocument newDomDoc;
							result = newDomDoc.setContent(&XMLGridFile);

							QDomElement tileoffset = newDomDoc.elementsByTagName("tileoffset").at(0).toElement();

							newTileElement->offsetGrid.first = tileoffset.attribute("x").toInt(); 
							newTileElement->offsetGrid.second = tileoffset.attribute("y").toInt();
							XMLGridFile.close();

	GridShapeContainer* newGrid = new GridShapeContainer;
	newGrid->AddCurves(PathXMLLines);
	element->GridLines = newGrid;
}
