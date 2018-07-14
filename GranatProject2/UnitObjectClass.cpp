#include "UnitObjectClass.h"



UnitObjectClass::UnitObjectClass()
{
}


UnitObjectClass::~UnitObjectClass()
{
}

void CoordClass::SetCoordDecart(int x, int y)
{
	DecPos(0) = x;
	DecPos(1) = y;
	IsoPos = n*DecPos;
}

void CoordClass::SetCoordIsometric(int x, int y)
{
	qDebug() << "POS HERO SET COORD - " << x << y;
	IsoPos(0) = x - 1;
	IsoPos(1) = y - 1;

	DecPos = m*IsoPos * 64;
	qDebug() << "IsoPos - " << x << y << "DecPos - " << DecPos(0) << DecPos(1);
	ScreenPos.x = DecPos(0);
	ScreenPos.y = 270 - DecPos(1);

}


inline void CoordClass::SetCoodScreen(double x, double y, int x_center, int y_center)
{
	DecPos(0) = x / 64.0 + x_center / 64;
	DecPos(1) = -(y - 350.0) / 64.0 + y_center / 64;

	IsoPos = n*DecPos;

	IsoPos(0) = floor(IsoPos(0));
	IsoPos(1) = floor(IsoPos(1));
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
	UnitImage.IterateAnimation();
}

void ClassHero::SetDestination(int x,int y )
{
	Destination.SetCoordIsometric(x, y);
}


void ClassHero::SetPosition(int x,int y )
{
	this->CurrentPosition.SetCoordIsometric(x, y);
	qDebug() << "Set position - " << x << y;
	this->UnitImage.spriteHero.setPosition(CurrentPosition.ScreenPos.x, CurrentPosition.ScreenPos.y);
}
