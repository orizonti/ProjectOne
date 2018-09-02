#include "UnitObjectClass.h"
#include "MapContainerClass.h"

MapContainerClass* UnitObjectClass::TerrainMap = 0;
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
	UnitImage = GroupImage(AnimationImage("MaceMan"), 4);
	UnitImage.SetPositionOnMap(CurrentPosition.GetIsoCoord());
	qDebug() << "================================================";
}


UnitObjectClass::~UnitObjectClass()
{
}

ClassWarriorUnit::ClassWarriorUnit(QString TypeUnit) : UnitObjectClass(TypeUnit)
{

}



void UnitObjectClass::MoveUnit()
{

	if (this->CurrentPosition == this->Destination)
		return;

	    //qDebug() << "MOVING " << this->CurrentPosition.GetIsoCoord() << "DESTINATION - " << Destination.GetIsoCoord();
	int dir_x = 0;
	int dir_y = 0;


	    if(d_x != 0)
    	dir_x = d_x / std::abs(d_x);
		if(d_y != 0)
		dir_y = d_y / std::abs(d_y);

			if (CurrentPosition.IsoPos(1) != RoutePoints.first().IsoPos(1))
				CurrentPosition.translate(0, -0.02*dir_y);
			else
				CurrentPosition.translate(-0.02*dir_x, 0);

			Direction Dir = Direction::Right;

			if (dir_y == 0)
			{
				if (dir_x == 1)
					Dir = Direction::Right;
			    if (dir_x == -1)
					Dir = Direction::Left;
			}

				if (dir_x == 0)
				{
						if (dir_y == 1)
							Dir = Direction::Up;
						if (dir_y == -1)
							Dir = Direction::Down;
				}

	UnitImage.SetDiretionMoving(Dir);

	UnitImage.SetPositionOnMap(CurrentPosition.IsoPos(0), CurrentPosition.IsoPos(1));
}

void UnitObjectClass::SetDestination(int x,int y  )
{
	RoutePoints.clear();
	qDebug() << "UNIT SET DESTINATION - " << x << y;
	Destination.SetCoordIsometric(x, y);


	 d_x = CurrentPosition.IsoPos(0) - Destination.IsoPos(0);
	 d_y = CurrentPosition.IsoPos(1) - Destination.IsoPos(1);

	 GameCoord RouteNode = CurrentPosition;
			   RouteNode.translate(0, d_y);
			   qDebug() << "ADD ROUTE NODE - " << RouteNode.GetIsoCoord();
	 RoutePoints.append(RouteNode);


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
