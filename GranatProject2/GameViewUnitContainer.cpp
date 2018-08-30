#include "GameViewUnitContainer.h"
GameViewUnitContainer::GameViewUnitContainer()
{
	qDebug() << "CREATE GAME UNIT CONTAINER";
	    QString GameDir = qgetenv("GAME_WORK_DIR");
		QString PathAnimation = GameDir + "/WORK_DIR/GameAnimation/";
	    Animation_Units_Objects = std::make_shared<AnimationSetContainer>(AnimationSetContainer());
	    Animation_Units_Objects->UploadAnimationSets(PathAnimation);
		UnitObjectClass::Animations = Animation_Units_Objects;

		auto TestUnit  = std::make_shared<UnitObjectClass>(UnitObjectClass("MaceMan"));

		UnitOnMapContainer.insert(TestUnit->CurrentPosition.GetIsoCoord(), TestUnit);


}
GameViewUnitContainer::~GameViewUnitContainer()
{
}

void GameViewUnitContainer::MapCellPressed(int x, int y)
{


}

void GameViewUnitContainer::DrawUnits(sf::RenderWindow &Window)
{
	for (auto Unit : UnitOnMapContainer.values())
	{
		Window.draw(Unit->UnitImage.CurrentSprite);
		Window.draw(Unit->UnitImage.CurrentSprite1);
		Window.draw(Unit->UnitImage.CurrentSprite2);
		Window.draw(Unit->UnitImage.CurrentSprite3);
		Unit->UnitImage.IterateAnimation(Direction::Right);
	}
}

