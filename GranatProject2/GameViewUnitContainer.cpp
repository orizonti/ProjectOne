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
		
		UnitOnMapContainer.insert(TestUnit->CurrentPosition.GetIsoCoord(), TestUnit);
		qDebug() << "GAME VIEW UNIT CONTAINER CREATE END";

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
		Unit->UnitImage.DisplayImage(Window);
	}
}

