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
}

void MapObject::SetPosition(int x, int y)
{
		CurrentPosition.SetCoordIsometric(x, y);
		ObjectImage->SetPositionOnMap(CurrentPosition.IsoPos(0), CurrentPosition.IsoPos(1));
};



void MapObject::MoveObjectOnStep()
{
				CurrentPosition.translate(MoveDirection.Step_Iso_X, MoveDirection.Step_Iso_Y);
				ObjectImage->Elevate(MoveDirection.Step_Elevation);
				ObjectImage->SetPositionOnMap(CurrentPosition.IsoPos(0), CurrentPosition.IsoPos(1));
}

void MapObject::MoveObjectOnStep(double Iso_Step_X, double Iso_Step_Y, double Lift_Step)
{
				CurrentPosition.translate(Iso_Step_X, Iso_Step_Y);
				ObjectImage->Elevate(Lift_Step);
				ObjectImage->SetPositionOnMap(CurrentPosition.IsoPos(0), CurrentPosition.IsoPos(1));
}

void MapObject::SetMoveDirection(double Iso_Step_X, double Iso_Step_Y, double Lift_Step)
{
	MoveDirection.Step_Iso_X = Iso_Step_X;
	MoveDirection.Step_Iso_Y = Iso_Step_Y;
	MoveDirection.Step_Elevation = Lift_Step;
}

void MapObject::LiftObject(double ElevationStep)
{
	ObjectImage->Elevate(ElevationStep);
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


void MapUnitObject::MoveObjectOnStep()
{
				CurrentPosition.translate(MoveDirection.Step_Iso_X, MoveDirection.Step_Iso_Y);
			    for (auto Object : MapObjects)
				Object->MoveObjectOnStep();
}

void MapUnitObject::MoveObjectOnStep(double Iso_Step_X, double Iso_Step_Y, double Lift_Step)
{
				CurrentPosition.translate(Iso_Step_X, Iso_Step_Y);
			    for (auto Object : MapObjects)
				Object->MoveObjectOnStep(Iso_Step_X, Iso_Step_Y, Lift_Step);
}

void MapUnitObject::ChangeMoveDirection()
{
		//CurrentPosition = Destination;
	int d_x = 0;
	int d_y = 0;
	int dir_x = 0;
	int dir_y = 0;
	if (RoutePoints.isEmpty())
	{
		 MoveDirection.SetMoveVector(0,0);
		 return;
	}

		Destination = RoutePoints.takeFirst();

	 d_x = Destination.IsoPos(0) - CurrentPosition.IsoPos(0);
	 d_y = Destination.IsoPos(1) - CurrentPosition.IsoPos(1);

	    if(d_x != 0)
    	dir_x = d_x / std::abs(d_x);
		if(d_y != 0)
		dir_y = d_y / std::abs(d_y);
		//qDebug() << "SET DIRECTION - " << MoveDirection.Unit_X << MoveDirection.Unit_Y << DirectionTable.value(QPair<int, int>(MoveDirection.Unit_X, MoveDirection.Unit_Y));


			MoveDirection.SetMoveVector(dir_x, dir_y);

			for (auto Object : MapObjects)
				Object->SetMoveDirection(MoveDirection.Step_Iso_X,MoveDirection.Step_Iso_Y,0);

	for(auto Object: MapObjects)
	Object->ObjectImage->SetObjectDirection(DirectionTable.value(QPair<int,int>(MoveDirection.Unit_X,MoveDirection.Unit_Y)));

}

void MapUnitObject::MoveUnit()
{


	if (this->CurrentPosition == this->Destination)
		ChangeMoveDirection();


	if (MoveDirection.IsNull())
		return;

			QVector<double> CellHeightMapCurrent;
			QVector<double> CellHeightMap;
	        

	if (CurrentPosition == NextCell)
	{
		CellHeightMapCurrent = this->TerrainMap->GetCellHeightMap(NextCell.IsoPos(0),NextCell.IsoPos(1));
		NextCell.translate(MoveDirection.Unit_X, MoveDirection.Unit_Y);

		CellHeightMap = this->TerrainMap->GetCellHeightMap(NextCell.IsoPos(0),NextCell.IsoPos(1));

		MapObjects[0]->SetMoveDirection(MoveDirection.Step_Iso_X,MoveDirection.Step_Iso_Y,(CellHeightMap[2] - CellHeightMapCurrent[2]) / 40.0);
		MapObjects[1]->SetMoveDirection(MoveDirection.Step_Iso_X,MoveDirection.Step_Iso_Y,(CellHeightMap[3] - CellHeightMapCurrent[3]) / 40.0);
		MapObjects[2]->SetMoveDirection(MoveDirection.Step_Iso_X,MoveDirection.Step_Iso_Y,(CellHeightMap[0] - CellHeightMapCurrent[0]) / 40.0);
		MapObjects[3]->SetMoveDirection(MoveDirection.Step_Iso_X,MoveDirection.Step_Iso_Y,(CellHeightMap[1] - CellHeightMapCurrent[1]) / 40.0);
	}

	this->MoveObjectOnStep();
}

void MapUnitObject::SetDestination(int x,int y  )
{
	qDebug() << "   ===========================================";
	qDebug() << "   CURRENT POS - " << CurrentPosition.GetIsoCoord();
	qDebug() << "   SET DESTINATION X - " << x << "Y - " << y;
	qDebug() << "   ===========================================";

	int d_x = 0;
	int d_y = 0;

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
	Object->SetPosition(x, y);
}



sf::Vector2f MapUnitObject::GetCoord()
{
	return CurrentPosition.GetIsoVector();
}
