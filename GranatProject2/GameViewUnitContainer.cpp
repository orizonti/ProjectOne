#include "GameViewUnitContainer.h"
GameViewUnitContainer::GameViewUnitContainer()
{
	    QString GameDir = qgetenv("GAME_WORK_DIR");
		QString PathAnimation = GameDir + "/WORK_DIR/GameAnimation/";
		AnimationImage::LoadAnimationSet(PathAnimation);


		auto TestUnit  = std::shared_ptr<MapUnitObject>(new MapUnitObject("MaceMan"));
		auto TestUnit2  = std::shared_ptr<MapUnitObject>(new MapUnitObject("MaceMan"));
		auto TestUnit3  = std::shared_ptr<MapUnitObject>(new MapUnitObject("MaceMan"));

		auto TestUnit11  = std::shared_ptr<MapUnitObject>(new MapUnitObject("MaceMan"));
		auto TestUnit22 = std::shared_ptr<MapUnitObject>(new MapUnitObject("MaceMan"));
		auto TestUnit33  = std::shared_ptr<MapUnitObject>(new MapUnitObject("MaceMan"));

		TestUnit->SetPosition(20, 21);
		TestUnit2->SetPosition(20, 22);
		TestUnit3->SetPosition(20, 23);
		
		TestUnit11->SetPosition(3, 6);
		TestUnit22->SetPosition(4, 6);
		TestUnit33->SetPosition(5, 6);

		UnitOnMapContainer.insert(TestUnit->CurrentPosition.GetIsoCoord(), TestUnit);
		UnitOnMapContainer.insert(TestUnit2->CurrentPosition.GetIsoCoord(), TestUnit2);
		UnitOnMapContainer.insert(TestUnit3->CurrentPosition.GetIsoCoord(), TestUnit3);

		UnitOnMapContainer.insert(TestUnit11->CurrentPosition.GetIsoCoord(), TestUnit11);
		UnitOnMapContainer.insert(TestUnit22->CurrentPosition.GetIsoCoord(), TestUnit22);
		UnitOnMapContainer.insert(TestUnit33->CurrentPosition.GetIsoCoord(), TestUnit33);

		MapUnitObject::DirectionTable.insert(QPair<int, int>( 0, 1), Direction::Right);
		MapUnitObject::DirectionTable.insert(QPair<int, int>( 0,-1), Direction::Left);
		MapUnitObject::DirectionTable.insert(QPair<int, int>( 1, 0), Direction::Up);
		MapUnitObject::DirectionTable.insert(QPair<int, int>(-1, 0), Direction::Down);

		MapUnitObject::DirectionTable.insert(QPair<int, int>(-1, 1), Direction::DownRight);
		MapUnitObject::DirectionTable.insert(QPair<int, int>(-1,-1), Direction::DownLeft);
		MapUnitObject::DirectionTable.insert(QPair<int, int>( 1,-1), Direction::UpRight);
		MapUnitObject::DirectionTable.insert(QPair<int, int>( 1, 1), Direction::UpLeft);


}
GameViewUnitContainer::~GameViewUnitContainer()
{
}

void GameViewUnitContainer::MapCellPressed(int x, int y)
{
	qDebug() << "MAP CELL PRESSED - " << x << y;
	if (UnitOnMapContainer.contains(QPair<float, float>(x, y)))
	{
		CurrentUnit = UnitOnMapContainer[QPair<float, float>(x, y)];
	}
	else
	{
		if (CurrentUnit != NULL)
		{
	    UnitOnMapContainer.remove(CurrentUnit->CurrentPosition.GetIsoCoord());
		CurrentUnit->SetDestination(x, y);
		UnitOnMapContainer.insert(CurrentUnit->EndDestination.GetIsoCoord(), CurrentUnit);

		qDebug() << "UNIT SET DESTINATION - " << x << y;
		}
	}

}

void GameViewUnitContainer::DrawUnits(sf::RenderWindow &Window)
{
	for (auto Unit : UnitOnMapContainer.values())
	{
		Unit->DrawObject(Window);
	}
}

void GameViewUnitContainer::MoveUnits()
{
	for (auto Unit : UnitOnMapContainer.values())
	{
		Unit->MoveUnit();
	}
}

