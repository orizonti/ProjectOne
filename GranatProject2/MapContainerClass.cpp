#include "MapContainerClass.h"


MapContainerClass::MapContainerClass()
{
	TileSet.CreateTileSetFromMap("Path to map");
}

void MapDisplayEngine::DrawMap()
{
	this->Map.DrawTerrain(this->Window);
}

void MapContainerClass::DrawTerrain(sf::RenderWindow &Window)
{
	for (QVector<TerrainObjectClass> Layer : TerrainLayers)
	{
			for (TerrainObjectClass item :Layer)
			{
			Window.draw(*item.GetSpriteToDraw());
			}

	}
}

void MapContainerClass::CreateMapFromFile(QString MapFilePath)
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

void MapDisplayEngine::KeyboardControl(sf::Event Keyboard)
{
			if (Keyboard.key.code == sf::Keyboard::Left)
			{
				Camera.move(-64, 0);
				OffsetCamera(0) += 1;
				qDebug() << "Scale - " << Scale << "OffsetCamera(0) - " << OffsetCamera(0);
			}

			if (Keyboard.key.code == sf::Keyboard::Right)
			{
				Camera.move(64.0, 0);
				OffsetCamera(0) -= 1;
				qDebug() << "Scale - " << Scale << "OffsetCamera(0) - " << OffsetCamera(0);
			}
			if (Keyboard.key.code == sf::Keyboard::Up)
			{
				Camera.move(0, -64);
				OffsetCamera(1) -= 1;
			}
			if (Keyboard.key.code == sf::Keyboard::Down)
			{
				Camera.move(0, 64);
				OffsetCamera(1) += 1;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				Camera.zoom(2);
				Scale = Scale / 2;

			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				Camera.zoom(0.5);
				Scale = Scale * 2;

			}
}
void MapDisplayEngine::MouseControl(sf::Event event)
{
			if (event.type == sf::Event::MouseMoved)
			{

				MousePosReal(0) = double(event.mouseMove.x - WindowSize.width() / 2) / (Scale*64.0) - OffsetCamera(0);
				MousePosReal(1) = double(event.mouseMove.y - WindowSize.height() / 2) / (Scale*64.0) - OffsetCamera(1);
				MousePosition.SetRealCoord(MousePosReal(0), MousePosReal(1));

			}
}
