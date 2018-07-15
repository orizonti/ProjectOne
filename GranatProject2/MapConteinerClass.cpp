#include "MapConteinerClass.h"

MapConteinerClass::MapConteinerClass()
{
	std::string Terrain1 = "E:/WorkDir/WORK_DIR/TERRAIN_ISOMETRIC_TILES/Tiles/with_grid/Grass2.png";
	std::string Hill1 = "E:/WorkDir/WORK_DIR/TERRAIN_ISOMETRIC_TILES/Tiles/with_grid/Grass2.png";

	this->UploadImage(Terrain1, 1);
	this->UploadImage(Hill1, 2);
}

void MapConteinerClass::DrawMap(sf::RenderWindow &Window)
{
//					IsoVect(0) = x;
//					IsoVect(1) = y;

//					DecVect = m*IsoVect * 256;

//					sprite.setPosition(0 + DecVect(0), 320 + DecVect(1));
//					window.draw(sprite);

	//Window.draw(this->spriteTerrain);

		//qDebug() << TerrainTypes.at(1).getPosition().x << TerrainTypes.at(1).getPosition().y;
	//TerrainTypes.at(1).setPosition(400, 400);
	Window.draw(TerrainTypes.at(1));
}


void MapConteinerClass::CreateMapFromFile(QString MapFilePath)
{
	
		QFile XMLMapFile(MapFilePath);
		bool result = XMLMapFile.open(QIODevice::ReadOnly);

		QDomDocument newDomDoc;
		result = newDomDoc.setContent(&XMLMapFile);
		QStringList DataStringLayer1;
		QStringList DataStringLayer2;



		if (result)
		{
			QDomNode currentNod = newDomDoc.documentElement().firstChild();
			while (!currentNod.isNull())
			{
				QDomElement newElement = currentNod.toElement();

				qDebug() << "*************************************";
				if (newElement.tagName() == "layer")
				{

				QDomNode dataNode = newElement.firstChild();

						if (newElement.attribute("name") == "Ground")
						{
							qDebug() << "GROUND LAYER";
							DataStringLayer1 = dataNode.toElement().text().split(QString("\r\n"));

							for (int y = 10; y<=100; y = y+10)
							{
							QString data = DataStringLayer1.at(110-y);

									for (int x = 0; x < data.size(); x = x + 2)
									{
										if (data.at(x).digitValue() == 1)
										{
											MapItem newItem;
											newItem.coord.first = x / 2;
											newItem.coord.second = y-1;
											newItem.type = 1;
											TerrainItems.append(newItem);

										}

									}

							}

						  for(MapItem item: TerrainItems)
						  qDebug() << "At coord - " << item.coord.first << item.coord.second << "terrain - " << item.type;
						}

						if (newElement.attribute("name") == "LandScape")
						{
							qDebug() << "LANDSCAPE LAYER";
							DataStringLayer2 = dataNode.toElement().text().split(QString("\r\n"));

							for (int y = 0; y <= 100; y++)
							{
							QString data = DataStringLayer2.at(100-y);

									for (int m = 0; m < data.size(); m = m + 2)
									{
										if (data.at(m).digitValue() == 2)
										{
											MapItem newItem;
											newItem.coord.first = m/2;
											newItem.coord.second = y;
											newItem.type = 2;
											LandScapeItems.append(newItem);

										}

									}

							}

						  for(MapItem item: LandScapeItems)
						  qDebug() << "At coord - " << item.coord.first << item.coord.second << "hill - " << item.type;
						}
				}

				currentNod = currentNod.nextSibling();
			}
		}


}

void MapConteinerClass::UploadImage(std::string ImagesFile, int Type)
{

	qDebug() << "Upload image type - " << Type;

		terrainImage.loadFromFile(ImagesFile);
		textureTerrain.loadFromImage(terrainImage);
		spriteTerrain.setTexture(textureTerrain);

		TerrainTypes.insert(std::pair<int, sf::Sprite>(Type, spriteTerrain));
}
