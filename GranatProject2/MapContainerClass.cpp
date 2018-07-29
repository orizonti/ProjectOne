#include "MapContainerClass.h"

MapDisplayEngine::~MapDisplayEngine()
{


}
MapDisplayEngine::MapDisplayEngine()
{
	 Window = new sf::RenderWindow(sf::VideoMode(1280, 640), "SFML works!");
	 Camera = new sf::View(sf::Vector2f(0, 0), sf::Vector2f(1280, 640));
	 CellSize = QSize(512, 256);
}

MapContainerClass::MapContainerClass()
{
	TileSet.CreateTileSetFromMap(GameDir +  + "/WORK_DIR/MAPS_TILED/Map512.tmx");
	
	this->CreateMapFromFile(GameDir +  + "/WORK_DIR/MAPS_TILED/Map512.tmx");
}

void MapDisplayEngine::DrawMap()
{
	Window->clear();
	this->Map.DrawTerrain(*Window);

	Window->setView(*Camera);
	Window->display();
}

void MapContainerClass::DrawTerrain(sf::RenderWindow &Window)
{
//	TileSet.TerrainElementsByType.value(15)->Sprite->setPosition(0, 0);
//	    Window.draw(*TileSet.TerrainElementsByType.value(22)->Sprite);

//		sf::RectangleShape shape(sf::Vector2f(100,100));
//		shape.setFillColor(sf::Color::Green);
//		shape.setPosition(0, 0);
//		Window.draw(shape);
	for (QVector<TerrainObjectClass*> Layer : TerrainLayers)
	{
			for (TerrainObjectClass* item :Layer)
			{
				item->DrawObject(Window);
			}

	}
}

void MapContainerClass::CreateMapFromFile(QString MapFilePath)
{
		qDebug() << "CREATE MAP FROM FILE - " << MapFilePath;
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

				if (newElement.tagName() == "layer" && newElement.attribute("name") != "GridHill" && newElement.attribute("name") != "Grid")
				{
					qDebug() << "ADD LAYER - " << newElement.attribute("name");
					TerrainLayers.insert(Number_Layer, QVector<TerrainObjectClass*>());

							QDomNode dataNode = newElement.firstChild();

							DataStringLayer = dataNode.toElement().text().split(QString("\r\n"));

							for (int y = 0; y<=60; y++)
							{
							QString data = DataStringLayer.at(y);
							QStringList Digits = data.split(",");
							int Type = 0;

								    for (int x = 0; x < Digits.size(); x++)
									{
										     
											 Type = Digits.at(x).toInt();
											 if (Type > 0)
											 {
											 //	qDebug() << Digits.at(x).toInt();
											 TerrainObjectClass* newItem = new TerrainObjectClass;
											 newItem->TerrainType = Type;
											 newItem->TerrainData = TileSet.TerrainElementsByType.value(newItem->TerrainType);
											 newItem->SetCoord(60-y, x);
											 TerrainLayers[Number_Layer].append(newItem);
											 }

									}

							}
							Number_Layer++;
				}

				currentNod = currentNod.nextSibling();
			}

		//	for (TerrainObjectClass* item : TerrainLayers[0])
		//	qDebug() << "At coord - " << item->Position.DecPos(0) << item->Position.DecPos(1) << "terrain - " << item->TerrainType;
		}

		qDebug() << "CREATE MAP END - " << MapFilePath;

}

void MapDisplayEngine::KeyboardControl(sf::Event Keyboard)
{
			if (Keyboard.key.code == sf::Keyboard::Left)
			{
				Camera->move(-CellSize.height(), 0);
				OffsetCamera(0) += 1;
				qDebug() << "Scale - " << Scale << "OffsetCamera(0) - " << OffsetCamera(0);
			}

			if (Keyboard.key.code == sf::Keyboard::Right)
			{
				Camera->move(CellSize.height() , 0);
				OffsetCamera(0) -= 1;
				qDebug() << "Scale - " << Scale << "OffsetCamera(0) - " << OffsetCamera(0);
			}
			if (Keyboard.key.code == sf::Keyboard::Up)
			{
				Camera->move(0, -CellSize.height());
				OffsetCamera(1) -= 1;
			}
			if (Keyboard.key.code == sf::Keyboard::Down)
			{
				Camera->move(0, CellSize.height());
				OffsetCamera(1) += 1;
			}

			if (Keyboard.key.code == sf::Keyboard::S)
			{
				Camera->zoom(2);
				Scale = Scale / 2;

			}
			if (Keyboard.key.code == sf::Keyboard::D)
			{
				Camera->zoom(0.5);
				Scale = Scale * 2;
			}
}
void MapDisplayEngine::MouseControl(sf::Event event)
{
				MousePosReal(0) = double(event.mouseMove.x - WindowSize.width() / 2) / (Scale*256.0) - OffsetCamera(0);
				MousePosReal(1) = double(event.mouseMove.y - WindowSize.height() / 2) / (Scale*256.0) - OffsetCamera(1);
				MousePosition.SetRealCoord(MousePosReal(0), MousePosReal(1));
				qDebug() << "MousePosReal - " << event.mouseMove.x << event.mouseMove.y;
			if (event.type == sf::Event::MouseMoved)
				this->Map.MapCellMoved(MousePosition.IsoPos(0), MousePosition.IsoPos(1));

			if (event.type == sf::Event::MouseButtonPressed)
			{
				this->Map.MapCellMoved(MousePosition.IsoPos(0), MousePosition.IsoPos(1));
				this->Map.MapCellPressed(MousePosition.IsoPos(0), MousePosition.IsoPos(1));
				this->Units.MapCellPressed(MousePosition.IsoPos(0), MousePosition.IsoPos(1));
			}


}

void MapContainerClass::MapCellPressed(int x, int y)
{
	//qDebug() << "MapCellPressed - " << x << y;
}

void MapContainerClass::MapCellMoved(int x, int y)
{
	//qDebug() << "MapCellMoved - " << x << y;
}
