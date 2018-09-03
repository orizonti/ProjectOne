#include "GameViewUnitContainer.h"
GameViewUnitContainer::GameViewUnitContainer()
{
	qDebug() << "CREATE GAME UNIT CONTAINER";
	    QString GameDir = qgetenv("GAME_WORK_DIR");
		QString PathAnimation = GameDir + "/WORK_DIR/GameAnimation/";
	    Animation_Units_Objects = std::shared_ptr<AnimationSetContainer>(new AnimationSetContainer());
	    Animation_Units_Objects->UploadAnimationSets(PathAnimation);
		AnimationImage::Animations = Animation_Units_Objects;

		auto TestUnit  = std::shared_ptr<UnitObjectClass>(new UnitObjectClass("MaceMan"));
		auto TestUnit2  = std::shared_ptr<UnitObjectClass>(new UnitObjectClass("Spearman"));
		auto TestUnit3  = std::shared_ptr<UnitObjectClass>(new UnitObjectClass("Axeman"));
		TestUnit2->SetPosition(10, 8);
		TestUnit3->SetPosition(12, 8);
		
		UnitOnMapContainer.insert(TestUnit->CurrentPosition.GetIsoCoord(), TestUnit);
		UnitOnMapContainer.insert(TestUnit2->CurrentPosition.GetIsoCoord(), TestUnit2);
		UnitOnMapContainer.insert(TestUnit3->CurrentPosition.GetIsoCoord(), TestUnit3);


		UnitObjectClass::DirectionTable.insert(QPair<int, int>( 0, 1), Direction::Right);
		UnitObjectClass::DirectionTable.insert(QPair<int, int>( 0,-1), Direction::Left);
		UnitObjectClass::DirectionTable.insert(QPair<int, int>( 1, 0), Direction::Up);
		UnitObjectClass::DirectionTable.insert(QPair<int, int>(-1, 0), Direction::Down);

		UnitObjectClass::DirectionTable.insert(QPair<int, int>(-1, 1), Direction::DownRight);
		UnitObjectClass::DirectionTable.insert(QPair<int, int>(-1,-1), Direction::DownLeft);
		UnitObjectClass::DirectionTable.insert(QPair<int, int>( 1,-1), Direction::UpRight);
		UnitObjectClass::DirectionTable.insert(QPair<int, int>( 1, 1), Direction::UpLeft);

		qDebug() << "UNIT HAS BEEN ADDED - " <<  TestUnit->CurrentPosition.GetIsoCoord();

}
GameViewUnitContainer::~GameViewUnitContainer()
{
}

void GameViewUnitContainer::MapCellPressed(int x, int y)
{

	if (UnitOnMapContainer.contains(QPair<float, float>(x, y)))
	{
		CurrentUnit = UnitOnMapContainer[QPair<float, float>(x, y)];
		if(CurrentUnit != NULL)
		qDebug() << "CURRENT UNIT - " << CurrentUnit->TypeUnit <<"POS- " << CurrentUnit->CurrentPosition.GetIsoCoord();
	}
	else
	{
		if(CurrentUnit != NULL)
		CurrentUnit->SetDestination(x, y);

	}



}

void GameViewUnitContainer::DrawUnits(sf::RenderWindow &Window)
{
	for (auto Unit : UnitOnMapContainer.values())
	{
		Unit->UnitImage.DisplayImage(Window);
	}
}

void GameViewUnitContainer::MoveUnits()
{
	for (auto Unit : UnitOnMapContainer.values())
	{
		Unit->MoveUnit();
	}
}

