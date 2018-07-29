#include "TerrainObjectClass.h"

TerrainObjectClass::TerrainObjectClass()
{

}


void TerrainObjectClass::SetCoord(int x, int y)
{
	if (TerrainData != 0)
	{
	Position.SetCoordIsometric(x, y);
	qDebug() << "ELEMENT -  "<< TerrainData->Name <<"COORD - " << x << y << "Dec - " << Position.DecPos(0) << Position.DecPos(1) << "Offset - " << this->TerrainData->offset.first << this->TerrainData->offset.second;
	}
}

sf::Sprite* TerrainObjectClass::GetSpriteToDraw()
{
	
	TerrainData->Sprite->setPosition(Position.DecPos(0) + TerrainData->offset.first,
		                             Position.DecPos(1) + TerrainData->offset.second - TerrainData->size.height());
	//qDebug() << "DrawType - " << TerrainType << "Offset - " << this->TerrainData->offset.first << this->TerrainData->offset.second;
	//TerrainData->Sprite->setPosition(Position.DecPos(0), Position.DecPos(1));

	return TerrainData->Sprite;
}

void TerrainObjectClass::DrawObject(sf::RenderWindow& Window)
{
	Window.draw(*this->GetSpriteToDraw());

	if (TerrainData->GridLines != 0)
	{
		TerrainData->GridLines->setPosition(this->Position.DecPos(0), this->Position.DecPos(1));
		Window.draw(*TerrainData->GridLines);
	}

}
