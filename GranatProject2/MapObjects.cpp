#include "MapObjects.h"
#include "MapContainerClass.h"

MapContainerClass* MapObject::TerrainMap = 0;
QMap<QPair<int, int>, Direction> MapUnitObject::DirectionTable;


MapObject::MapObject()
{
}

MapObject::MapObject(QString Type)
{
	ObjectImage = std::shared_ptr<AnimationImage>(new AnimationImage(Type));
};

void MapObject::SetElevationStep(double Step)
{
	ElevationStep = Step;
}

void MapObject::LiftObject(double Step)
{
	ObjectImage->Elevate(Step);
}

void MapObject::DrawObject(sf::RenderWindow& Window)
{
	ObjectImage->DisplayImage(Window);
}

MapGroupObject::MapGroupObject()
{
}

void MapGroupObject::DrawObject(sf::RenderWindow& Window)
{
	for (auto Object : MapObjects)
		Object->DrawObject(Window);
}

MapGroupObject::MapGroupObject(QString TypeObject, int size)
{
	CurrentPosition.SetCoordIsometric(0, 0);
	Type = TypeObject;

	this->GroupSize = size;
	for (int n = 0; n < GroupSize; n++)
	{
		MapObjects.append(std::shared_ptr<MapObject>(new MapObject(Type)));
		MapObjects.last()->CurrentPosition = CurrentPosition;
	}

		MapObjects[0]->ObjectImage->OffsetToImage = QPair<float,float>(0.5,0);   //BR
		MapObjects[1]->ObjectImage->OffsetToImage = QPair<float,float>(1,0);     //TR
		MapObjects[2]->ObjectImage->OffsetToImage = QPair<float,float>(0.5,-0.5);//BL
		MapObjects[3]->ObjectImage->OffsetToImage = QPair<float,float>(1,-0.5);  //TL


}


MapLiveObject::MapLiveObject(QString Type, int size) : MapGroupObject(Type,size)
{
}

MapUnitObject::MapUnitObject(QString TypeObject,int Size) : MapLiveObject(TypeObject,Size)
{
	qDebug() << "CREATE UNIT of TYPE - " << TypeObject;

	Destination = CurrentPosition;

}


MapUnitObject::~MapUnitObject()
{
	qDebug() << "UNIT DESCTRUCTOR ";
}



void MapUnitObject::GetMoveDirection()
{
	 dir_x = 0;
	 dir_y = 0;
	 if (RoutePoints.isEmpty())
		 return;

		Destination = RoutePoints.takeFirst();

	 d_x = Destination.IsoPos(0) - CurrentPosition.IsoPos(0);
	 d_y = Destination.IsoPos(1) - CurrentPosition.IsoPos(1);


	    if(d_x != 0)
    	dir_x = d_x / std::abs(d_x);
		if(d_y != 0)
		dir_y = d_y / std::abs(d_y);

		//qDebug() << "SET DIRECTION - " << dir_x << dir_y << DirectionTable.value(QPair<int, int>(dir_x, dir_y));

	for(auto Object: MapObjects)
	Object->ObjectImage->SetObjectDirection(DirectionTable.value(QPair<int,int>(dir_x,dir_y)));

}

void MapUnitObject::MoveUnit()
{

	if (this->CurrentPosition == this->Destination)
	{
		CurrentPosition = Destination;
		GetMoveDirection();
	}

	if (dir_y == 0 && dir_x == 0)
		return;

			QVector<double> CellHeightMapCurrent;
			QVector<double> CellHeightMap;
	        

			if (CurrentPosition == NextCell)
			{
				CellHeightMapCurrent = this->TerrainMap->GetCellHeightMap(NextCell.IsoPos(0),NextCell.IsoPos(1));
				NextCell.translate(dir_x, dir_y);

				CellHeightMap = this->TerrainMap->GetCellHeightMap(NextCell.IsoPos(0),NextCell.IsoPos(1));

			//Images[0]->Elevate(Translation[2]);
			//Images[1]->Elevate(Translation[3]);

			//Images[2]->Elevate(Translation[0]);
	        //Images[3]->Elevate(Translation[1]);
					if (!CellHeightMap.isEmpty())
					{
						if (CellHeightMapCurrent.isEmpty())
						{
								MapObjects[0]->SetElevationStep(CellHeightMap[2] / 40.0);
								MapObjects[1]->SetElevationStep(CellHeightMap[3] / 40.0);
								MapObjects[2]->SetElevationStep(CellHeightMap[0] / 40.0);
								MapObjects[3]->SetElevationStep(CellHeightMap[1] / 40.0);
						}
						else
						{
								MapObjects[0]->SetElevationStep((CellHeightMap[2] - CellHeightMapCurrent[2]) / 40.0);
								MapObjects[1]->SetElevationStep((CellHeightMap[3] - CellHeightMapCurrent[3]) / 40.0);
								MapObjects[2]->SetElevationStep((CellHeightMap[0] - CellHeightMapCurrent[0]) / 40.0);
								MapObjects[3]->SetElevationStep((CellHeightMap[1] - CellHeightMapCurrent[1]) / 40.0);
						}
					}
			}


			if (dir_y != 0)
				CurrentPosition.translate(0, 0.025*dir_y);

			if (dir_x != 0)
				CurrentPosition.translate(0.025*dir_x, 0);


			for (auto Object : MapObjects)
			{
				Object->LiftObject(Object->ElevationStep);
				Object->ObjectImage->SetPositionOnMap(CurrentPosition.IsoPos(0), CurrentPosition.IsoPos(1));
			}
}

void MapUnitObject::SetDestination(int x,int y  )
{
	qDebug() << "   ===========================================";
	qDebug() << "   CURRENT POS - " << CurrentPosition.GetIsoCoord();
	qDebug() << "   SET DESTINATION X - " << x << "Y - " << y;
	qDebug() << "   ===========================================";


	 RoutePoints.clear();
	 EndDestination.SetCoordIsometric(x, y);

	 int diff = 0;
	 d_x = EndDestination.IsoPos(0) - CurrentPosition.IsoPos(0);
	 d_y = EndDestination.IsoPos(1) - CurrentPosition.IsoPos(1);

	 GameCoord RouteNode = CurrentPosition;
			   RouteNode.translate(0, d_y);
	 RoutePoints.append(RouteNode);
	 RoutePoints.append(EndDestination);
	 Destination = CurrentPosition;
	 NextCell = CurrentPosition;


}


void MapUnitObject::SetPosition(int x,int y  )
{
	this->CurrentPosition.SetCoordIsometric(x, y);

	for (auto Object : MapObjects)
		Object->ObjectImage->SetPositionOnMap(CurrentPosition.IsoPos(0), CurrentPosition.IsoPos(1));
}



sf::Vector2f MapUnitObject::GetCoord()
{
	return CurrentPosition.GetIsoVector();
}
