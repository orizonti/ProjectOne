#include "TerrainObjectClass.h"

TerrainObjectClass::TerrainObjectClass(TerrainTileElement* Terrain)
{
	if (Terrain != 0)
	{
		if (Terrain->GridLines != 0)
		{
		PathContour = Terrain->GridLines->GetPathContour();
		StripesPath = Terrain->GridLines->GetStripePathes();
		}
		TerrainData = Terrain;
	}

}


void TerrainObjectClass::SetCoord(int x, int y)
{
	if (TerrainData != 0)
	{
	Position.SetCoordIsometric(x, y);
	PathContour.translate(Position.DecPos(0), Position.DecPos(1));
	ShapeContour.AddCurves(PathContour);
	qDebug() << "ELEMENT -  "<< TerrainData->Name <<"COORD - " << x << y << "Dec - " << Position.DecPos(0) << Position.DecPos(1) << "Offset - " << this->TerrainData->offset.first << this->TerrainData->offset.second;
	}
}

sf::Sprite* TerrainObjectClass::GetSpriteToDraw()
{
	
	TerrainData->Sprite->setPosition(Position.DecPos(0) + TerrainData->offset.first,
		                             Position.DecPos(1) + TerrainData->offset.second - TerrainData->size.height()+128);
	//qDebug() << "DrawType - " << TerrainType << "Offset - " << this->TerrainData->offset.first << this->TerrainData->offset.second;
	//TerrainData->Sprite->setPosition(Position.DecPos(0), Position.DecPos(1));

	return TerrainData->Sprite;
}

void TerrainObjectClass::DrawObject(sf::RenderWindow& Window)
{
	Window.draw(*this->GetSpriteToDraw());
}


void TerrainObjectClass::DrawGrid(sf::RenderWindow& Window)
{

	if (TerrainData->GridLines != 0)
	{
		TerrainData->GridLines->SetPosition(this->Position.DecPos(0) + TerrainData->offset.first, 
			                                this->Position.DecPos(1) + TerrainData->offset.second - TerrainData->size.height()+128);
		TerrainData->GridLines->DrawGrid(Window);

		if (FLAG_MOUSE_MOVED)
			Window.draw(ShapeContour);

//			this->TerrainData->GridLines->DrawContour(Window);
	}
}

bool TerrainObjectClass::CheckCursorPosition(int x, int y)
{
	QPair<int, int> CoordPointer;
	CoordPointer.first = 0;
	CoordPointer.second = 0;

	QPainterPath path_line;

	//qDebug() << "=============     ===================";
	    bool result = PathContour.contains(QPointF(x, y));

		if(result)
			qDebug() << "COORD - " << x << y << "CONTAINS IN - " << TerrainType << "AT POS - " << this->Position.GetIsoCoord() << result;

		this->FLAG_MOUSE_MOVED = result;

//	for (int n = 0; n < TerrainData->GridLines->PathLineHoriz.size(); n++)
//	{
//		path_line = TerrainData->GridLines->PathLineHoriz.at(n).translated(Position.DecPos(0), Position.DecPos(1));
//		if (path_line.contains(QPointF(x, y)))
//			CoordPointer.first = n;
//	}

//	for (int n = 0; n < TerrainData->GridLines->PathLineVert.size(); n++)
//	{
//		path_line = TerrainData->GridLines->PathLineVert.at(n).translated(Position.DecPos(0), Position.DecPos(1));
//		if (path_line.contains(QPointF(x, y)))
//			CoordPointer.second = n;
//	}

	return result;
}
