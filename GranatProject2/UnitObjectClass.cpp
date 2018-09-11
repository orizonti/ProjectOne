#include "UnitObjectClass.h"
#include "MapContainerClass.h"

MapContainerClass* UnitObjectClass::TerrainMap = 0;
QMap<QPair<int, int>, Direction> UnitObjectClass::DirectionTable;


std::shared_ptr<AnimationSetContainer> UnitObjectClass::Animations = NULL;

void UnitObjectClass::CheckHeightMap(int x, int y)
{

}


UnitObjectClass::UnitObjectClass(QString Type )
{
	qDebug() << "================================================";
	qDebug() << "CREATE UNIT of TYPE - " << Type;
	TypeUnit = Type;
	CurrentPosition.SetCoordIsometric(4, 8);
	Destination.SetCoordIsometric(4, 8);


	qDebug() << "CREATE GROUP IMAGE TO UNIT";
	UnitImage = GroupImage(AnimationImage(Type), 4);
	UnitImage.SetPositionOnMap(CurrentPosition.GetIsoCoord());
	qDebug() << "================================================";
	Elevations.resize(4);
	//Elevations.fill(0, 4);
	Elevations.fill(0);
	qDebug() << Elevations[0];

	qDebug() << "================================================";
}


UnitObjectClass::~UnitObjectClass()
{
	qDebug() << "UNIT DESCTRUCTOR ";
}

ClassWarriorUnit::ClassWarriorUnit(QString TypeUnit) : UnitObjectClass(TypeUnit)
{

}


void UnitObjectClass::GetMoveDirection()
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

	UnitImage.SetDiretionMoving(DirectionTable.value(QPair<int,int>(dir_x,dir_y)));

}

void UnitObjectClass::MoveUnit()
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
			    PosInCell = 0;

				if (CellHeightMap.isEmpty())
					Elevations.fill(0);
				else
				{
					if (CellHeightMapCurrent.isEmpty())
					{
				Elevations[0] = CellHeightMap[0]/40.0;
				Elevations[1] = CellHeightMap[1]/40.0;
				Elevations[2] = CellHeightMap[2]/40.0;
				Elevations[3] = CellHeightMap[3]/40.0;
					}
					else
					{
				Elevations[0] = (CellHeightMap[0] - CellHeightMapCurrent[0])/40.0;
				Elevations[1] = (CellHeightMap[1] - CellHeightMapCurrent[1])/40.0;
				Elevations[2] = (CellHeightMap[2] - CellHeightMapCurrent[2])/40.0;
				Elevations[3] = (CellHeightMap[3] - CellHeightMapCurrent[3])/40.0;
					}
				}
			}


			if (dir_y != 0)
				CurrentPosition.translate(0, 0.025*dir_y);

			if (dir_x != 0)
				CurrentPosition.translate(0.025*dir_x, 0);

			if(Elevations.last() != 0)
			UnitImage.TranslateElevation(Elevations);

			PosInCell += 0.025;
			CurrentHeight += Elevations[0];






	UnitImage.SetPositionOnMap(CurrentPosition.IsoPos(0), CurrentPosition.IsoPos(1));
}

void UnitObjectClass::SetDestination(int x,int y  )
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


void UnitObjectClass::SetPosition(int x,int y  )
{
	this->CurrentPosition.SetCoordIsometric(x, y);
	this->UnitImage.SetPositionOnMap(CurrentPosition.GetIsoCoord());
}


void ClassWarriorUnit::Attack()
{

}

sf::Vector2f UnitObjectClass::GetCoord()
{
	return CurrentPosition.GetIsoVector();
}
