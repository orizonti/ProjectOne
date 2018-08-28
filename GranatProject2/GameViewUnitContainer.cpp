#include "GameViewUnitContainer.h"
GameViewUnitContainer::GameViewUnitContainer()
{
	qDebug() << "CREATE GAME UNIT CONTAINER";
	    QString GameDir = qgetenv("GAME_WORK_DIR");
		QString PathAnimation = GameDir + "/WORK_DIR/GameAnimation/";
	this->Animation_Units_Objects.UploadAnimationSets(PathAnimation);
}
GameViewUnitContainer::~GameViewUnitContainer()
{
}

void GameViewUnitContainer::MapCellPressed(int x, int y)
{


}

void GameViewUnitContainer::DrawUnits(sf::RenderWindow &Window)
{

}

