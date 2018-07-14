#include "MapConteinerClass.h"

void MapConteinerClass::DrawMap(sf::RenderWindow &Window)
{
	qDebug() << "Draw map not implemented";
}


void MapConteinerClass::CreateMapFromFile(QString MapFilePath)
{
	
		QFile XMLMapFile(MapFilePath);
		bool result = XMLMapFile.open(QIODevice::ReadOnly);

		QDomDocument newDomDoc;
		result = newDomDoc.setContent(&XMLMapFile);
		QStringList DataStringLayer1;
		QStringList DataStringLayer2;

		QVector<MapItem> TerrainItems;
		QVector<MapItem> LandScapeItems;


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

							for (int n = 100; n>=0; n = n-10)
							{
							QString data = DataStringLayer1.at(n);

									for (int m = 0; m < data.size(); m = m + 2)
									{
										if (data.at(m).digitValue() == 1)
										{
											MapItem newItem;
											newItem.coord.first = n-10;
											newItem.coord.second = m / 2;
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

							for (int y = 100; y > 0; y--)
							{
							QString data = DataStringLayer2.at(y);

									for (int m = 0; m < data.size(); m = m + 2)
									{
										if (data.at(m).digitValue() == 2)
										{
											MapItem newItem;
											newItem.coord.first = m/2;
											newItem.coord.second = y-1;
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
