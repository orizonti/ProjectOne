#include "MapConteinerClass.h"

MapConteinerClass::MapConteinerClass()
{
	TileSet.CreateTileSetFromMap("Path to map");
}

void MapConteinerClass::DrawMap(sf::RenderWindow &Window)
{
	for (QVector<TerrainObjectClass> Layer : TerrainLayers)
	{
			for (TerrainObjectClass item :Layer)
			{
			Window.draw(*item.GetSpriteToDraw());
			}

	}

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
					qDebug() << "ADD LAYER - " << newElement.attribute("name");
					TerrainLayers.insert(Number_Layer, QVector<TerrainObjectClass>());

				QDomNode dataNode = newElement.firstChild();

							DataStringLayer = dataNode.toElement().text().split(QString("\r\n"));

							for (int y = 1; y<=100; y++)
							{
							QString data = DataStringLayer.at(y);

									for (int x = 0; x < data.size(); x = x + 2)
									{
											TerrainObjectClass newItem;
											 newItem.TerrainType = data.at(x).digitValue();
											 newItem.Position.SetCoordIsometric(x / 2, y);
											 newItem.TerrainData = TileSet.TerrainElementsByType.at(newItem.TerrainType);
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

