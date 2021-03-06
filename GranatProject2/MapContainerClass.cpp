#include "MapContainerClass.h"
sf::Font Font;


MapContainerClass::MapContainerClass()
{
	CurrentCenterCluster.first = 1;
	CurrentCenterCluster.second = 1;


		//===============================================================================================
		GameCoord Point0; Point0.SetCoordIsometric(0, 0);    //CREATING RED RHOMBUS SHAPE THAT IS USED TO HIGHLIGTH CELL IS POINTED BY MOUSE CURSOR
		GameCoord Point1; Point1.SetCoordIsometric(0, 1);
		GameCoord Point2; Point2.SetCoordIsometric(1, 1);
		GameCoord Point3; Point3.SetCoordIsometric(1, 0);
		QVector<CurveShape> QuadeLines;
		CurveShape Line1;  Line1.AddLine(Point0.GetDecVector(), Point1.GetDecVector(),0);
		CurveShape Line2;  Line2.AddLine(Point1.GetDecVector(), Point2.GetDecVector(),1);
		CurveShape Line3;  Line3.AddLine(Point2.GetDecVector(), Point3.GetDecVector(),0);
		CurveShape Line4;  Line4.AddLine(Point3.GetDecVector(), Point0.GetDecVector(),1);

		QuadeLines.append(Line1);
		QuadeLines.append(Line2);
		QuadeLines.append(Line3);
		QuadeLines.append(Line4);

		BorderCell.SetQuadeShapes(QuadeLines);
		BorderCell.SetColor(sf::Color::Red);

		BorderCellPressed = BorderCell;
		BorderCellPressed.SetColor(sf::Color::Yellow);
		//===============================================================================================

		//===============================================================================================
		GameCoord StartLine;   //CREATING BLACK LINES WITH WITH AND HEIGHT OF GLOBAL MAP THAT IS USED TO DRAW GRID ON MAP
		GameCoord EndLine;
		for (int x = 0; x < 60; x++)
		{
			StartLine.SetCoordIsometric(x, 0);
			EndLine.SetCoordIsometric(x, 59);

		  CurveShape Line;
		  Line.AddLine(StartLine.GetDecVector(), EndLine.GetDecVector(),0);
		  PlainGridLines.append(Line);
		}
		//------------------------------------------------------------------------------------------------

		for (int y = 0; y < 60; y++)
		{
			StartLine.SetCoordIsometric(0, y);
			EndLine.SetCoordIsometric(59, y);

		  CurveShape Line;
		  Line.AddLine(StartLine.GetDecVector(), EndLine.GetDecVector(),1);
		  PlainGridLines.append(Line);
		}
		//===============================================================================================


		//===============================================================================================
		TerrainObjectClass::LoadTileSet();
	this->CreateMapFromFile(GameDir +  + "/WORK_DIR/MAPS_TILED/Map512.tmx");      // CREATING TERRAIN OBJECTS THAT ARE USED TO DRAWING MAP, EACH TERRAINS OBJECT HAS IT'S COORD AND LINK TO RESPECTIVE TILE
	TerrainClasterization(this->TerrainLayers.value(1));  // CLASTERIZATION HILLS LAYER
		//===============================================================================================

	MapUnitObject::TerrainMap = this;
}

void MapContainerClass::DrawCurrentCell(sf::RenderWindow &Window)
{
	if (!FLAG_CURSOR_ON_HILL) // DRAW RED RHOMBUS ON CURRENT PLAIN CELL, IF CURSOV MOVING ON HILL CURVE RED CELL IS DRAWING IN TERRAIN TERRAIN DRAWING METHOD DRAW
	{
	BorderCell.SetPosition(CursorPosition.DecPos(0), CursorPosition.DecPos(1));


	BorderCell.DrawShape(Window);
	}
	BorderCellPressed.DrawShape(Window);
}

void MapContainerClass::DrawTerrain(sf::RenderWindow &Window)
{

			QVector<TerrainObjectClass*> Ground = TerrainLayers.value(0);
			QVector<TerrainObjectClass*> Hill = TerrainLayers.value(1);

//===============================================================================================
			for (TerrainObjectClass* item :Ground)   //DRAW PLAIN TERRAIN
				item->DrawObject(Window);
//------------------------------------------------------------------------
			
			for (CurveShape& Shape : PlainGridLines) //DRAW PLAIN RHOMBUS GRID 
				Window.draw(Shape);
//------------------------------------------------------------------------
			for (TerrainObjectClass* item :Hill)     //DRAW HILL TERRAIN OBJECTS
				item->DrawObject(Window);
//===============================================================================================
			for (QVector<TerrainObjectClass*> Layer : TerrainLayers)  //DRAW CURVE GRID ON HILL
			{
					for (TerrainObjectClass* item :Layer)
						item->DrawGrid(Window);
			}

			for (TerrainObjectClass* item :Hill)    //DRAW HILL TERRAIN OBJECTS
				item->DrawTerrainHeight(Window);
//===============================================================================================
	    if(FLAG_CURSOR_ON_HILL)  //DRAW RED BORDER AROUND CURRENT HILL'S CLUSTER, IT MUST BE REMOVED, USED TO DEBUGING
		Window.draw(ConvexToClusters.value(CurrentCenterCluster));

}

void MapContainerClass::CreateMapFromFile(QString MapFilePath)
{
	    // CREATING TERRAIN OBJECTS THAT ARE USED TO DRAWING MAP, EACH TERRAINS OBJECT HAS IT'S COORD AND LINK TO RESPECTIVE TILE
		//===============================================================================================
		QFile XMLMapFile(MapFilePath);
		bool result = XMLMapFile.open(QIODevice::ReadOnly); // OPEN XML FILE THAT CONTAINS TEXT DATA OF TERRAIN POSITION AND TILE TYPES

		QDomDocument newDomDoc;
		result = newDomDoc.setContent(&XMLMapFile);
		QStringList DataStringLayer;

		int Number_Layer = 0;

		//===============================================================================================
		if (result)
		{
			//ITERATE THROGH XML DOM NODE STRUCTURE AND FOR ALL LAYER NODE CREATE VECTOR MASSIVE OF TERRAIN OBJECTS
			QDomNode currentNod = newDomDoc.documentElement().firstChild();
			while (!currentNod.isNull())
			{
				QDomElement newElement = currentNod.toElement();

				if (newElement.tagName() == "layer" && newElement.attribute("name") != "GridHill" && newElement.attribute("name") != "Grid")
				{
					//GRID LAYERS IS NOT USED BECAUSE PLAING GRID IS DRAWED BY PROGRAMMING METHOD WITH CURVE SHAPE
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
											 TerrainObjectClass* newItem = new TerrainObjectClass(Type);
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

		}


}

QVector<double> MapContainerClass::GetCellHeightMap(int x, int y)
{
	GameCoord Coord; 
	Coord.SetCoordIsometric(x, y);
    PairCoord CenterCluster = DefineBelongPoint(CalculateNearestCluster(x, y),Coord);

	QVector<double> Heights;

	if (CenterCluster != PairCoord(0, 0))
	{
			for (auto Terrain : ClusteredObjects.value(CenterCluster))
			{
				     if (Terrain->ContainsMapPoint(x, y))
					 Heights = Terrain->GetHeightMapOnCell(x,y);

					 if (!Heights.isEmpty())
						 return Heights;

			}
	}
	Heights.resize(4); Heights.fill(0);

	return Heights;

}

void MapContainerClass::DefineCellMoved(int x, int y)
{

	for (auto Terrain : ClusteredObjects.value(CurrentCenterCluster))
	{
		//CHECK THAT ANY TERRAIN OBJECT COINTAINS CUSOR COORD IN IT's BOUND
		//IF ANY TERRAIN CONTAINS 
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


QPair<int, int> MapContainerClass::GetRealCellPressed()
{

	if (FLAG_CURSOR_ON_HILL)
		return CurrentTerrain->GetCellPressed();
	else
		return PressedPosition.GetIsoCoord();
}

void MapContainerClass::MapCellPressed(int x, int y)
{
	PressedPosition.SetCoordIsometric(x, y);

	if (CurrentTerrain != 0)
	BorderCellPressed = CurrentTerrain->GetCellBorderMoved();
	else
	{

	BorderCellPressed = BorderCell;
	BorderCellPressed.SetPosition(PressedPosition.DecPos(0), PressedPosition.DecPos(1));
	}

	BorderCellPressed.SetColor(sf::Color::Yellow);

}

void MapContainerClass::MapCellMoved(int x, int y)
{
	//GET CURRENT CURSOR POINT AND DEFINE NEAREST CLUSTER CENTER, THEN CHECK IF CURSOR POINT COINTAINS IN BOUND OF CLUSTER,
	//IF CURSOR IS IN BOUND OF CLUSTER, THAN FLAG_MOUSE_MOVED IS TRUE, IT IS SIGNAL TO DRAW CURRENT RED CELL IN HILL TILE INSTEAD OF PLAIN CELL RHOMBUS BORDER
	GameCoord Coord; 
	Coord.SetCoordIsometric(x, y);
    CurrentCenterCluster = DefineBelongPoint(CalculateNearestCluster(x, y),Coord);
	CursorPosition.SetCoordIsometric(x, y);


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

	//TERRAIN CLASTERIZATION TO DEFINE HILL UNDER CURSOR LATER
	//=================================================================================
	//=================================================================================
	std::function<Terrains(Terrains)> GetCornerCoords = [](Terrains Cluster) -> Terrains
	{
		//GET CORNERS OF HILL'S CLUSTER TO CREATE CLUSTER BORDER
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
	//--------------------------------------------------------------------------------------------------------
				std::function<QPainterPath(sf::ConvexShape)> ConvertConvexToPainterPath = 
					[](sf::ConvexShape ShapeBorder) -> QPainterPath
				{

				    //CONVERT CONVEX QUADRANGLE TO PAINTER_PATH THAH IS USED TO DEFINE IS CURSOR POINT TO HILL
					QPolygonF Polygon;
					for(int n = 0; n < ShapeBorder.getPointCount();n++)
					{
						QPointF newPoint;
						newPoint.setX(ShapeBorder.getPoint(n).x);
						newPoint.setY(ShapeBorder.getPoint(n).y);
						Polygon << newPoint;
					}
					QPainterPath PainterPath; 
					PainterPath.addPolygon(Polygon);
					PainterPath.closeSubpath();
					return PainterPath;

				};

	//=================================================================================
	//=================================================================================
				std::function<sf::ConvexShape(Terrains)> CreateClusterConvex = 
					[](Terrains	CornerTerrains) -> sf::ConvexShape
				{

					//CREATE QUADERANGLE TO DRAW RED BORDER WHEN CURSOR POINT TO HILL TERRAINS CLUSTER
					sf::ConvexShape newConvex;
					newConvex.setPointCount(4);
					//qDebug() << "CREATE CLUSTER CONVEX";
				 GameCoord PointCoord;
							for (auto TerrainObject : CornerTerrains)
							{                                          //GET CORNER TYPE TERRAIN ELEMENT 5,7,2 or 22 AND SET IT'S COORD TO CONVEX
								switch (TerrainObject->TerrainType)
								{
								case 5:

									PointCoord = TerrainObject->Position;
									PointCoord.translate(3, 0);
				                    newConvex.setPoint(0,PointCoord.GetDecVector());
									break;
								case 7:
									PointCoord = TerrainObject->Position;
									PointCoord.translate(3, 3);
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
	
	//=================================================================================
	//=================================================================================
	std::function<PairCoord(Terrains)> CalcCentroid = [](Terrains CornerTerrains) -> PairCoord
	{

		//CENTROID OF CLUSTER IS USED TO LOCATE NEAREST TO CURSOR HILL CLUSTER IN MAP<COORD,CLUSTER>
		PairCoord Centroid;
		PairCoord TR;
		PairCoord TL;
		PairCoord BR;
		PairCoord BL;

		double x1, y1;
		double x2, y2;
		double x3, y3;
		double x4, y4;


	//--------------------------------------------------------------------------------------------------------
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
	//--------------------------------------------------------------------------------------------------------

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
	//=================================================================================
	//=================================================================================
	std::function<QVector<TerrainObjectClass*>(QVector<TerrainObjectClass*>)> DefineCloseObject = 
		[&TerrainLayer,&DefineCloseObject](QVector<TerrainObjectClass*> SeedObjects) -> QVector<TerrainObjectClass*>
	    {
		QVector<TerrainObjectClass*> NewSet;
		//GET ARBITRARY TERRAIN ELEMENT, GET CLOSEST ELEMENTS TO THIS ELEMENT, APPEND ITS TO CLUSTER SET
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
					TerrainLayer.remove(n);      //REMOVE ELEMENTS FROM COMMON SET AND PLACE IT TO CLUSTER SET
					NewSet.append(DefineObject);
				}
				
			}
		}
		  //--------------------------------------------------------------------------------------------------------

		
				QVector<TerrainObjectClass*> ChildSet;
        //GET AVAIBLE CLUSTER SET OF TERRAINS AND RECURSIVELY DEFINE CLOSEST TERRAINS TO EACH ELEMENTS IN AVAIBLE SET
	    //IF ELEMENTS WITHIN MIN LENGHT BOUND IS NOT AVABILE THEN CLUSTER SET IS FULL 
						if (NewSet.size() != 0)
						{
						ChildSet = DefineCloseObject(NewSet);
						NewSet.append(ChildSet);
						}

				//qDebug() << "CHILD SET UNDEFINED CODE INVOKE";
				return NewSet;
		};
	   //=================================================================================
	   //=================================================================================

	while (TerrainLayer.size() != 0)
	{
	QVector<TerrainObjectClass*> StartSeed;
    StartSeed.append(TerrainLayer.takeFirst());                              //TAKE FIRST TERRAIN OBJECT
	QVector<TerrainObjectClass*> NewCluster = DefineCloseObject(StartSeed);  //DEFINE CLOSEST TERRAIN TO THIS OBJECTS
							     NewCluster.prepend(StartSeed.first());      //THOSE TERRAIS IS CLUSTER

	Terrains CornerTerrains = GetCornerCoords(NewCluster);                   //GETTING CORNERS OF TERRAINS CLUSTER TO CALC CENTROID AND CREATE BORDER OF CLUSTER

	PairCoord Centroid = CalcCentroid(CornerTerrains);                       //CALCULATING CENTROID OF CLASTER THAT WILL BE USED TO DEFINE CURRENT OR NEAREST CLUSTER TO CURSOR POSITION

	sf::ConvexShape convex = CreateClusterConvex(CornerTerrains);            //CREATING QUADERANGLE CONVEX TO DRAWING BORDER OF CURRENT CLASTER 

	ClusteredObjects.insert(Centroid, NewCluster);
	ConvexToClusters.insert(Centroid, convex);
	PathToClusters.insert(Centroid, ConvertConvexToPainterPath(convex));     //CREATING PATH_PAINTER QUADERANGLE THAT WILL BE USED TO DEFINE THAT CURSOR POINT TO HILL

	}

}

PairCoord MapContainerClass::CalculateNearestCluster(int x, int y)
{
	//CALCULATE LENGTH TO ALL CENTERS CLUSTER AND TAKE MIN LENGTH

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
		Lengths.insert(length,Center);
	}

	PairCoord NearestCenter = Lengths.values().first();
	return NearestCenter;
}

PairCoord MapContainerClass::DefineBelongPoint(PairCoord NearestCenter, GameCoord& Coord)
{
	//TAKE NEAREST TO CURSOR CLUSTER AND CHECK IF POINT CONTAINS IN CLUSTER BOUND
	PairCoord Null; Null.first = 0; Null.second = 0;

	QPainterPath path = PathToClusters.value(NearestCenter);
	QPointF Point; 
	Point.setX(Coord.DecPos(0));
	Point.setY(Coord.DecPos(1));

	if (path.contains(Point))
	return NearestCenter;

	return Null;
}
