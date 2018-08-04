#include "MapContainerClass.h"

MapDisplayEngine::~MapDisplayEngine()
{


}
MapDisplayEngine::MapDisplayEngine()
{
	 Window = new sf::RenderWindow(sf::VideoMode(1280, 640), "SFML works!");
	 Camera = new sf::View(sf::Vector2f(0, 0), sf::Vector2f(1280, 640));
	 CellSize = QSize(512, 256);
	Window->setView(*Camera);
	OffsetCamera(0) = 0;
	OffsetCamera(1) = 0;
	WindowSize.setHeight(640);
	WindowSize.setWidth(1280);
}

MapContainerClass::MapContainerClass()
{

		BorderCellTexture.loadFromFile((GameDir + "/WORK_DIR/TERRAIN_ISOMETRIC_TILES/Tiles/1x1Border.png").toStdString());
		BorderCellSprite.setTexture(BorderCellTexture);

	bool res = img.loadFromFile((GameDir + "/WORK_DIR/TERRAIN_ISOMETRIC_TILES/Tiles/1x1Terrain.png").toStdString());
	texture.loadFromImage(img);
	sprite.setTexture(texture);

	TileSet.CreateTileSetFromMap(GameDir +  + "/WORK_DIR/MAPS_TILED/Map512.tmx");
	this->CreateMapFromFile(GameDir +  + "/WORK_DIR/MAPS_TILED/Map512.tmx");
	TerrainClasterization(this->TerrainLayers.value(1));
}

void MapDisplayEngine::DrawMap()
{
	Window->clear();
	this->Map.DrawTerrain(*Window);
	this->Map.DrawCurrentCell(*Window);

	Window->setView(*Camera);
	Window->display();
}
void MapContainerClass::DrawCurrentCell(sf::RenderWindow &Window)
{

	BorderCellSprite.setPosition(CursorPosition2.DecPos(0), CursorPosition2.DecPos(1)-128);
	//BorderCellSprite.setPosition(0,0);
	Window.draw(BorderCellSprite);
}

void MapContainerClass::DrawTerrain(sf::RenderWindow &Window)
{

//	for (int x = 0; x <= 40; x++)
//	{
//		for (int y = 0; y <= 40; y++)
//		{
//			CursorPosition.SetCoordIsometric(x, y);
//			sprite.setPosition(CursorPosition.DecPos(0),CursorPosition.DecPos(1)  - 128);
//			Window.draw(sprite);
//		}
//	}


	for (QVector<TerrainObjectClass*> Layer : TerrainLayers)
	{
			for (TerrainObjectClass* item :Layer)
			{
				item->DrawObject(Window);

				if(FLAG_DRAW_GRID)
				item->DrawGrid(Window);
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
					qDebug() << "ADD LAYER - " << newElement.attribute("name") << "NUMBER - " << Number_Layer;
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
				qDebug() << "Scale - " << Scale << "OffsetCamera(1) - " << OffsetCamera(0);
			}
			if (Keyboard.key.code == sf::Keyboard::Up)
			{
				Camera->move(0, -CellSize.height());
				OffsetCamera(1) += 1;
			}
			if (Keyboard.key.code == sf::Keyboard::Down)
			{
				Camera->move(0, CellSize.height());
				OffsetCamera(1) -= 1;
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

			if (Keyboard.key.code == sf::Keyboard::G)
			{
				Map.FLAG_DRAW_GRID = true;
			}

			if (Keyboard.key.code == sf::Keyboard::H)
			{
				Map.FLAG_DRAW_GRID = false;
			}
}
void MapDisplayEngine::MouseControl(sf::Event event)
{
	            double x_pos_real = double(event.mouseMove.x - WindowSize.width() / 2) / (CellSize.height()*Scale) - OffsetCamera(0);
				double y_pos_real = double(event.mouseMove.y - WindowSize.height() / 2) / (CellSize.height()*Scale) - OffsetCamera(1);
				MousePosition.SetRealCoord(x_pos_real,y_pos_real);

			if (event.type == sf::Event::MouseMoved)
				this->Map.MapCellMoved(MousePosition.IsoPos(0), MousePosition.IsoPos(1));

			if (event.type == sf::Event::MouseButtonPressed)
			{
				this->Map.MapCellPressed(MousePosition.IsoPos(0), MousePosition.IsoPos(1));
				this->Units.MapCellPressed(MousePosition.IsoPos(0), MousePosition.IsoPos(1));
			}


}

void MapContainerClass::MapCellPressed(int x, int y)
{
	qDebug() << "MapCellPressed - " << x << y;
}

void MapContainerClass::MapCellMoved(int x, int y)
{
	qDebug() << "MapCellMoved - " << x << y;
	CursorPosition2.SetCoordIsometric(x, y);
}
void MapContainerClass::TerrainClasterization(QVector<TerrainObjectClass*> TerrainLayer)
{

	qDebug() << "========================================";
	qDebug() << "========================================";
	qDebug() << "TERRAIN CLASTERIZATION";

	std::function<QPair<double,double>(Terrains)> CalcMassCenter = [](Terrains Cluster) -> PairCoord
	{
			double summ_x = 0;
			double summ_y = 0;
		for (auto Object : Cluster)
		{
			summ_x += Object->Position.IsoPos(0);
			summ_y += Object->Position.IsoPos(1);
		}
		QPair<double,double> Center;
		Center.first = summ_x / Cluster.size();
		Center.second = summ_y / Cluster.size();

		qDebug() << "CLUSTER - " << Cluster[0]->Position.IsoPos(0) << Cluster[0]->Position.IsoPos(1) << "CENTER - " << Center.first << Center.second;
		return Center;
	};

	std::function<PairCoord(PairCoord, Terrains)> DefineCornerObject = [](PairCoord Center, Terrains Cluster)->PairCoord
	{
		QMap<double, TerrainObjectClass*> ObjectsByLength;
		QVector<double> MassLength;
			double d_x = 0;
			double d_y = 0;
			double Length = 0;
		for (auto Object : Cluster)
		{
			d_x =Center.first - Object->Position.IsoPos(0);
			d_y =Center.second - Object->Position.IsoPos(1);
			Length = std::hypot(d_x, d_y);
			ObjectsByLength.insert(Length,Object);
			//qDebug() << "LENGHT - "<<Length << Object->TerrainType;
		}


		//qSort(ObjectsByLength);

		
		PairCoord CornerCoord; 
		CornerCoord.first = ObjectsByLength.last()->Position.IsoPos(0);
	    CornerCoord.second=  ObjectsByLength.last()->Position.IsoPos(1);

		qDebug() << "CORNER OBJECT - "<< CornerCoord.first << CornerCoord.second<< "Type - " << ObjectsByLength.last()->TerrainType;
		return CornerCoord;
	};

	std::function<QVector<TerrainObjectClass*>(QVector<TerrainObjectClass*>)> DefineCloseObject = 
		[&TerrainLayer,&DefineCloseObject](QVector<TerrainObjectClass*> SeedObjects) -> QVector<TerrainObjectClass*>
	    {
		QVector<TerrainObjectClass*> NewSet;

		double d_x = 0;
		double d_y = 0;
		double lenght = 0;
		for (auto Object : SeedObjects)
		{
			for(int n = 0; n < TerrainLayer.size(); n++)
			{
				TerrainObjectClass* DefineObject = TerrainLayer.at(n);
				d_x = Object->Position.IsoPos(0) - DefineObject->Position.IsoPos(0);
				d_y = Object->Position.IsoPos(1) - DefineObject->Position.IsoPos(1);
				lenght = std::hypot(d_x, d_y);

				if (lenght < 8)
				{
				//	qDebug() << DefineObject->Position.IsoPos(0) << 
				//		        DefineObject->Position.IsoPos(1) << 
				//		        "TYPE - " << DefineObject->TerrainType << "LENGHT - " << lenght;
					TerrainLayer.remove(n);
					NewSet.append(DefineObject);
				}
				
			}
		}

		
		QVector<TerrainObjectClass*> ChildSet;

		if (NewSet.size() != 0)
		{
		ChildSet = DefineCloseObject(NewSet);
		NewSet.append(ChildSet);
		}

		return NewSet;
		};
		

	while (TerrainLayer.size() != 0)
	{
	QVector<TerrainObjectClass*> StartSeed;
    StartSeed.append(TerrainLayer.takeFirst());
    qDebug() << "*********************************************";
	QVector<TerrainObjectClass*> NewCluster = DefineCloseObject(StartSeed);
							     NewCluster.prepend(StartSeed.first());
    qDebug() << "*********************************************";
	qDebug() << "========================================";
	QPair<double,double> CenterCoord =  CalcMassCenter(NewCluster);
	PairCoord OffsetFromClustCenter = DefineCornerObject(CenterCoord, NewCluster);

	ClusteredObjects.insert(CenterCoord, NewCluster);
	ClustersOffsetFromCenter.insert(CenterCoord, OffsetFromClustCenter);
	qDebug() << "-----------------          -------------";
	qDebug() << "CREATED NEW CLUSTER SIZE - " << NewCluster.size();
	qDebug() << "========================================";
	}

}
