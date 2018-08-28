#include "UnitObjectClass.h"



UnitObjectClass::UnitObjectClass(QString TypeUnit)
{
}


UnitObjectClass::~UnitObjectClass()
{
}

ClassHero::ClassHero(QString TypeUnit) : UnitObjectClass(TypeUnit)
{

}



void ClassHero::MoveHero()
{

	if (this->CurrentPosition == this->Destination)
		return;



	double x1 = this->CurrentPosition.DecPos(0);
	double y1 = this->CurrentPosition.DecPos(1);


	double x2 = Destination.DecPos(0);
	double y2 = Destination.DecPos(1);

	this->CurrentPosition.DecPos(0) = CurrentPosition.DecPos(0) + 4;
	this->CurrentPosition.DecPos(1) = (-(x1*y2 - x2*y1) - (y1 - y2)*(CurrentPosition.DecPos(0))) / (x2 - x1);

	UnitImage.SetPositionImage(CurrentPosition.DecPos(0), CurrentPosition.DecPos(1));
	UnitImage.IterateAnimation(Direction::Left);
}

void ClassHero::SetDestination(int x,int y )
{
	Destination.SetCoordIsometric(x, y);
}


void ClassHero::SetPosition(int x,int y )
{
	this->CurrentPosition.SetCoordIsometric(x, y);
	this->UnitImage.CurrentSprite.setPosition(CurrentPosition.DecPos(0),CurrentPosition.DecPos(1));
}


