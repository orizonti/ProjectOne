#include "MapConteinerClass.h"

MapConteinerClass::MapConteinerClass()
{
	TileSet.CreateTileSetFromMap("Path to map");
//	qDebug() << "CREATE MAP CONTAINER";
//	std::string GameDir = qgetenv("GAME_WORK_DIR");

//	std::string Terrain1 =GameDir + "/WORK_DIR/TERRAIN_ISOMETRIC_TILES/Tiles/with_grid/Grass2.png";
//	std::string Hill1 = GameDir + "/WORK_DIR/TERRAIN_ISOMETRIC_TILES/Tiles/with_grid/Hill1.png";
//	qDebug() << "Terrain1 - " << Terrain1.c_str();

//	this->UploadImage(Terrain1, 1);
//	this->UploadImage(Hill1, 2);

//	m << 1, 1,
//		-0.5, 0.5;

//	n << 0.5, -1,
//		0.5, 1;
}

void MapConteinerClass::DrawMap(sf::RenderWindow &Window)
{
//	for (MapItem item : TerrainItems)
//	{

//					IsoVect(0) = item.coord.first;
//					IsoVect(1) = item.coord.second;
//					DecVect = m*IsoVect * 128;
//	TerrainTypes.at(1)->setPosition(DecVect(0), 320 + DecVect(1));
//	Window.draw(*TerrainTypes.at(1));
//	}


//	for (MapItem item : LandScapeItems)
//	{

//					IsoVect(0) = item.coord.first;
//					IsoVect(1) = item.coord.second;
//					DecVect = m*IsoVect * 128;
//	TerrainTypes.at(2)->setPosition(DecVect(0), 320 + DecVect(1));
//	Window.draw(*TerrainTypes.at(2));
//	}

}


void MapConteinerClass::CreateMapFromFile(QString MapFilePath)
{
	
		QFile XMLMapFile(MapFilePath);
		bool result = XMLMapFile.open(QIODevice::ReadOnly);

		QDomDocument newDomDoc;
		result = newDomDoc.setContent(&XMLMapFile);
		QStringList DataStringLayer;

		int Number_Layer = 0;

		if (result)
		{
			QDomNode currentNod = newDomDoc.documentElement().firstChild();
			while (!currentNod.isNull())
			{
				QDomElement newElement = currentNod.toElement();

				qDebug() << "*************************************";
				if (newElement.tagName() == "layer")
				{
					TerrainLayers.insert(Number_Layer, QVector<TerrainObjectClass>());

				QDomNode dataNode = newElement.firstChild();

							DataStringLayer = dataNode.toElement().text().split(QString("\r\n"));

							for (int y = 1; y<=100; y++)
							{
							QString data = DataStringLayer.at(y);

									for (int x = 0; x < data.size(); x = x + 2)
									{
										//Change CoordDecard to CoordScreen
											TerrainObjectClass newItem;
											 newItem.TerrainType = data.at(x).digitValue();

											 newItem.Position.SetCoordDecart(x / 2, y);
											TerrainLayers[Number_Layer].append(newItem);

									}

							}


							for (TerrainObjectClass item : TerrainLayers[Number_Layer])
						  qDebug() << "At coord - " << item.Position.DecPos(0) << item.Position.DecPos(1) << "terrain - " << item.TerrainType;

				}

				currentNod = currentNod.nextSibling();
			}
		}


}

void MapConteinerClass::UploadImage(std::string ImagesFile, int Type)
{

}
