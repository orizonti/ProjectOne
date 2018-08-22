#include "MapContainerClass.h"

MapDisplayEngine::~MapDisplayEngine()
{


}
MapDisplayEngine::MapDisplayEngine()
{
	 Window = new sf::RenderWindow(sf::VideoMode(1800, 1000), "SFML works!");
	 Camera = new sf::View(sf::Vector2f(0, 0), sf::Vector2f(1800, 1000));
	 CellSize = QSize(512, 256);
	Window->setView(*Camera);
	OffsetCamera(0) = 0;
	OffsetCamera(1) = 0;
	WindowSize.setHeight(1000);
	WindowSize.setWidth(1800);

}

MapContainerClass::MapContainerClass()
{



	CurrentCenterCluster.first = 1;
	CurrentCenterCluster.second = 1;

		BorderCellTexture.loadFromFile((GameDir + "/WORK_DIR/TERRAIN_ISOMETRIC_TILES/Tiles/1x1Border.png").toStdString());
		BorderCellSprite.setTexture(BorderCellTexture);

		GameCoord Point0; Point0.SetCoordIsometric(0, 0);
		GameCoord Point1; Point1.SetCoordIsometric(0, 1);
		GameCoord Point2; Point2.SetCoordIsometric(1, 1);
		GameCoord Point3; Point3.SetCoordIsometric(1, 0);
		QVector<CurveShape> QuadeLines;
CurveShape Line1;
CurveShape Line2;
CurveShape Line3;
CurveShape Line4;

		  Line1.AddLine(Point0.GetDecVector(), Point1.GetDecVector(),0);
		  Line2.AddLine(Point1.GetDecVector(), Point2.GetDecVector(),1);
		  Line3.AddLine(Point2.GetDecVector(), Point3.GetDecVector(),0);
		  Line4.AddLine(Point3.GetDecVector(), Point0.GetDecVector(),1);

		QuadeLines.append(Line1);
		QuadeLines.append(Line2);
		QuadeLines.append(Line3);
		QuadeLines.append(Line4);

		BorderCell.SetQuadeShapes(QuadeLines);
		BorderCell.SetColor(sf::Color::Red);

		GameCoord StartLine;
		GameCoord EndLine;
		for (int x = 0; x < 60; x++)
		{
			StartLine.SetCoordIsometric(x, 0);
			EndLine.SetCoordIsometric(x, 59);

		  CurveShape Line;
		  Line.AddLine(StartLine.GetDecVector(), EndLine.GetDecVector(),0);
		  PlainGridLines.append(Line);
		}

		for (int y = 0; y < 60; y++)
		{
			StartLine.SetCoordIsometric(0, y);
			EndLine.SetCoordIsometric(59, y);

		  CurveShape Line;
		  Line.AddLine(StartLine.GetDecVector(), EndLine.GetDecVector(),1);
		  PlainGridLines.append(Line);
		}


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
	if (!FLAG_CURSOR_ON_HILL)
	{
	BorderCell.SetPosition(CursorPosition2.DecPos(0), CursorPosition2.DecPos(1));
	//BorderCellSprite.setPosition(0,0);
	BorderCell.DrawShape(Window);

	}
}

void MapContainerClass::DrawTerrain(sf::RenderWindow &Window)
{


			QVector<TerrainObjectClass*> Ground = TerrainLayers.value(0);

			QVector<TerrainObjectClass*> Hill = TerrainLayers.value(1);

			for (TerrainObjectClass* item :Ground)
				item->DrawObject(Window);

					if (FLAG_DRAW_GRID)
					{
					for (CurveShape& Shape : PlainGridLines)
						Window.draw(Shape);
					}

			for (TerrainObjectClass* item :Hill)
				item->DrawObject(Window);




	for (QVector<TerrainObjectClass*> Layer : TerrainLayers)
	{
			for (TerrainObjectClass* item :Layer)
			{

				if(FLAG_DRAW_GRID)
				item->DrawGrid(Window);

			}

	}

	    if(FLAG_CURSOR_ON_HILL)
		Window.draw(ConvexToClusters.value(CurrentCenterCluster));

}

void MapContainerClass::CreateMapFromFile(QString MapFilePath)
{
		//qDebug() << "CREATE MAP FROM FILE - " << MapFilePath;
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
											 TerrainObjectClass* newItem = new TerrainObjectClass(TileSet.TerrainElementsByType.value(Type));
											 newItem->TerrainType = Type;
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

		//qDebug() << "CREATE MAP END - " << MapFilePath;

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

void MapContainerClass::DefineCellMoved(int x, int y)
{

	for (auto Terrain : ClusteredObjects.value(CurrentCenterCluster))
	{

		if (FLAG_CURSOR_ON_HILL)
		{
		 if(Terrain->CheckCursorPosition(x, y))
			this->CurrentTerrain = Terrain;

		}
		else
		{
			if (this->CurrentTerrain != 0)
			{
			this->CurrentTerrain->FLAG_MOUSE_MOVED = false;
			this->CurrentTerrain = 0;
			}
		}

	}
}



void MapDisplayEngine::MouseControl(sf::Event event)
{
	            double x_pos_real = double(event.mouseMove.x - WindowSize.width() / 2) / (CellSize.height()*Scale) - OffsetCamera(0);
				double y_pos_real = double(event.mouseMove.y - WindowSize.height() / 2) / (CellSize.height()*Scale) - OffsetCamera(1);
				MousePosition.SetRealCoord(x_pos_real,y_pos_real);

				if (event.type == sf::Event::MouseMoved)
				{
				this->Map.MapCellMoved(MousePosition.IsoPos(0), MousePosition.IsoPos(1));
				Map.DefineCellMoved(MousePosition.MousePosReal(0), MousePosition.MousePosReal(1));
				}

			if (event.type == sf::Event::MouseButtonPressed)
			{
				this->Map.MapCellPressed(MousePosition.IsoPos(0), MousePosition.IsoPos(1));
				this->Units.MapCellPressed(MousePosition.IsoPos(0), MousePosition.IsoPos(1));
			}


}

void MapContainerClass::MapCellPressed(int x, int y)
{
	CursorPosition2.SetCoordIsometric(x, y);

}

void MapContainerClass::MapCellMoved(int x, int y)
{

	GameCoord Coord; 
	Coord.SetCoordIsometric(x, y);
    CurrentCenterCluster = DefineBelongPoint(CalculateNearestCluster(x, y),Coord);
	CursorPosition2.SetCoordIsometric(x, y);


	if (CurrentCenterCluster != PairCoord(0, 0))
	{
		FLAG_CURSOR_ON_HILL = true;
	}
	else
	{

		    FLAG_CURSOR_ON_HILL = false;

			if (this->CurrentTerrain != 0)
			{
			this->CurrentTerrain->FLAG_MOUSE_MOVED = false;
			this->CurrentTerrain = 0;
			}
	}

}
void MapContainerClass::TerrainClasterization(QVector<TerrainObjectClass*> TerrainLayer)
{

	//qDebug() << "========================================";
	//qDebug() << "========================================";
	//qDebug() << "TERRAIN CLASTERIZATION";


	std::function<Terrains(Terrains)> GetCornerCoords = [](Terrains Cluster) -> Terrains
	{
		Terrains CornerTerrains;
		for (auto TerrainObject : Cluster)
		{
			if (TerrainObject->TerrainType == 5 ||
				TerrainObject->TerrainType == 7 ||
				TerrainObject->TerrainType == 22 ||
				TerrainObject->TerrainType == 2)
				CornerTerrains.append(TerrainObject);
		}
		return CornerTerrains;
	};

				std::function<QPainterPath(sf::ConvexShape)> ConvertConvexToPainterPath = 
					[](sf::ConvexShape ShapeBorder) -> QPainterPath
				{

					//qDebug() << "=============================================";
				
					QPolygonF Polygon;
					for(int n = 0; n < ShapeBorder.getPointCount();n++)
					{
						QPointF newPoint;
						newPoint.setX(ShapeBorder.getPoint(n).x);
						newPoint.setY(ShapeBorder.getPoint(n).y);
						Polygon << newPoint;
						//qDebug() << "ADD POINT - " << newPoint;
					}
					QPainterPath PainterPath; 
					PainterPath.addPolygon(Polygon);
					PainterPath.closeSubpath();
					//qDebug() << "=============================================";
					return PainterPath;

				};

				std::function<sf::ConvexShape(Terrains)> CreateClusterConvex = 
					[](Terrains	CornerTerrains) -> sf::ConvexShape
				{

					sf::ConvexShape newConvex;
				 newConvex.setPointCount(4);

				 GameCoord PointCoord;
							for (auto TerrainObject : CornerTerrains)
							{
								switch (TerrainObject->TerrainType)
								{
								case 5:

									PointCoord = TerrainObject->Position;
									PointCoord.translate(3, 0);
				                    newConvex.setPoint(0,PointCoord.GetDecVector());
									break;
								case 7:
									PointCoord = TerrainObject->Position;
									PointCoord.translate(4, 2);
				                    newConvex.setPoint(1,PointCoord.GetDecVector());
									break;
								case 2:
									PointCoord = TerrainObject->Position;
									PointCoord.translate(0, 3);
				                    newConvex.setPoint(2,PointCoord.GetDecVector());
									break;
								case 22:
				                    newConvex.setPoint(3,TerrainObject->Position.GetDecVector());
									break;
								}
							};
							newConvex.setFillColor(sf::Color::Transparent);
							newConvex.setOutlineColor(sf::Color::Red);
							newConvex.setOutlineThickness(20);

				 return newConvex;
				};
	
	std::function<PairCoord(Terrains)> CalcCentroid = [](Terrains CornerTerrains) -> PairCoord
	{


		PairCoord Centroid;
		PairCoord TR;
		PairCoord TL;
		PairCoord BR;
		PairCoord BL;

		double x1, y1;
		double x2, y2;
		double x3, y3;
		double x4, y4;


		for (auto TerrainObject : CornerTerrains)
		{
			switch (TerrainObject->TerrainType)
			{
			case 5:
				TR = TerrainObject->Position.GetIsoCoord();
				break;
			case 7:
				TL = TerrainObject->Position.GetIsoCoord();
				break;
			case 2:
				BL = TerrainObject->Position.GetIsoCoord();
				break;
			case 22:
				BR = TerrainObject->Position.GetIsoCoord();
				break;
			}
		};

			x1 = (BL.first + TL.first) / 2;
			y1 = (BL.second + TL.second) / 2;

			x2 = (BR.first + TR.first) / 2;
			y2 = (BR.second + TR.second) / 2;

			x3 = (TL.first + TR.first) / 2;
			y3 = (TL.second + TR.second) / 2;
			
			x4 = (BL.first + BR.first) / 2;
			y4 = (BL.second + BR.second) / 2;

			double A = x1*y2 - y1*x2;
			double B = x3*y4 - y3*x4;
			double C = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);

			Centroid.first = (A*(x3 - x4) - B*(x1 - x2))/C +3;
			Centroid.second = (A*(y3 - y4) - B*(y1 - y2))/C +2;

		return Centroid;
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
   // qDebug() << "*********************************************";
	QVector<TerrainObjectClass*> NewCluster = DefineCloseObject(StartSeed);
							     NewCluster.prepend(StartSeed.first());
   // qDebug() << "*********************************************";
	//qDebug() << "========================================";

	Terrains CornerTerrains = GetCornerCoords(NewCluster);

	PairCoord Centroid = CalcCentroid(CornerTerrains);

	sf::ConvexShape convex = CreateClusterConvex(CornerTerrains);

	ClusteredObjects.insert(Centroid, NewCluster);
	CornersCluster.insert(Centroid, CornerTerrains);
	ConvexToClusters.insert(Centroid, convex);
	PathToClusters.insert(Centroid, ConvertConvexToPainterPath(convex));

	//qDebug() << "-----------------          -------------";
	//qDebug() << "CREATED NEW CLUSTER SIZE - " << NewCluster.size();
	//qDebug() << "========================================";
	}

}

PairCoord MapContainerClass::CalculateNearestCluster(int x, int y)
{
	QList<PairCoord> ClusterCenters = ClusteredObjects.keys();
	QMap<double,PairCoord> Lengths;
	double d_x = 0;
	double d_y = 0;

	double length = 0;
	for (auto Center : ClusterCenters)
	{
		d_x = Center.first - x;
		d_y = Center.second - y;
		length = std::hypot(d_x, d_y);
		//qDebug() <<"CENTER - " << Center<< " LENGTH - "  << length;
		Lengths.insert(length,Center);
	}

	PairCoord NearestCenter = Lengths.values().first();
	//qDebug() << "MIN LENGHT - " << NearestCenter;
	return NearestCenter;
}

PairCoord MapContainerClass::DefineBelongPoint(PairCoord NearestCenter, GameCoord Coord)
{
	PairCoord Null; Null.first = 0; Null.second = 0;

	QPainterPath path = PathToClusters.value(NearestCenter);
	QPointF Point; 
	Point.setX(Coord.DecPos(0));
	Point.setY(Coord.DecPos(1));

	if (path.contains(Point))
	{
	//qDebug() << "POINT BELONG TO NEAREST - " << NearestCenter;
	return NearestCenter;
	}
	else
	//qDebug() << "POINT NOT BELONG TO NEAREST - " << NearestCenter;

	return Null;
}
