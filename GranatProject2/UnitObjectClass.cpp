#include "UnitObjectClass.h"
#include "MapContainerClass.h"

MapContainerClass* UnitObjectClass::TerrainMap = 0;
std::shared_ptr<AnimationSetContainer> UnitObjectClass::Animations = NULL;

void UnitObjectClass::CheckHeightMap(int x, int y)
{

}

UnitObjectClass::UnitObjectClass(QString Type )
{
	qDebug() << "CREATE UNIT of TYPE - " << Type;
	TypeUnit = Type;
	CurrentPosition.SetCoordIsometric(8, 8);
	Destination.SetCoordIsometric(8, 8);

	UnitAnimation = Animations->GetUnitAnimationSet(Animations->UnitsType.value(Type));
	UnitImage.AnimationSet = UnitAnimation; // it must be rewrited !!!!!

	if (UnitAnimation == NULL)
	{
		qDebug() << "NO ANIMATION";
		return;
	}

	UnitImage.SetTexture(UnitAnimation->GetTexture(Direction::Right, 19));
	UnitImage.SetPositionImage(CurrentPosition.GetDecCoord());
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



	double x1 = this->CurrentPosition.DecPos(0);
	double y1 = this->CurrentPosition.DecPos(1);


	double x2 = Destination.DecPos(0);
	double y2 = Destination.DecPos(1);

	this->CurrentPosition.DecPos(0) = CurrentPosition.DecPos(0) + 4;
	this->CurrentPosition.DecPos(1) = (-(x1*y2 - x2*y1) - (y1 - y2)*(CurrentPosition.DecPos(0))) / (x2 - x1);

	UnitImage.SetPositionImage(CurrentPosition.DecPos(0), CurrentPosition.DecPos(1));
	UnitImage.IterateAnimation(Direction::Left);
}

void UnitObjectClass::SetDestination(int x,int y  )
{
	Destination.SetCoordIsometric(x, y);
}


void UnitObjectClass::SetPosition(int x,int y  )
{
	this->CurrentPosition.SetCoordIsometric(x, y);
	this->UnitImage.CurrentSprite.setPosition(CurrentPosition.DecPos(0),CurrentPosition.DecPos(1));
}


void ClassWarriorUnit::Attack()
{

}

sf::Vector2i UnitObjectClass::GetCoord()
{
	return CurrentPosition.GetIsoVector();
}
