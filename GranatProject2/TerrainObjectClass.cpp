#include "TerrainObjectClass.h"

TerrainObjectClass::TerrainObjectClass()
{

}


void TerrainObjectClass::SetCoord(int x, int y)
{
	//Position.SetCoodScreen(x, y);
}

sf::Sprite* TerrainObjectClass::GetSpriteToDraw()
{
	sf::Sprite* SpriteToDraw = this->TerrainData->Sprite;
	SpriteToDraw->setPosition(Position.ScreenPos.x, Position.ScreenPos.y);
	return SpriteToDraw;
}
