#include "TerrainObjectClass.h"

int TerrainObjectClass::OFFSET_HEIGHT_TEXT = -20;
TerrainObjectClass::TerrainObjectClass(TerrainTileElement* Terrain)
{
	if (Terrain != 0)
	{
		if (Terrain->GridLines != 0)
		{
		PathContour = Terrain->GridLines->GetPathContour();
		CellPathes = Terrain->GridLines->GetSubCells();
		}
		TerrainData = Terrain;
	}

}


void TerrainObjectClass::SetCoord(int x, int y)
{
	if (TerrainData != 0)
	{
	Position.SetCoordIsometric(x, y);
	PathContour.translate(Position.DecPos(0) + TerrainData->offset.first, 
		                  Position.DecPos(1) + TerrainData->offset.second - TerrainData->size.height()+128);
	for (QPainterPath& Path : CellPathes)
	{
	Path.translate(Position.DecPos(0) + TerrainData->offset.first, 
		           Position.DecPos(1) + TerrainData->offset.second - TerrainData->size.height()+128);
	}

	//qDebug() << "ELEMENT -  "<< TerrainData->Name <<"COORD - " << x << y << "Dec - " << Position.DecPos(0) << Position.DecPos(1) << "Offset - " << this->TerrainData->offset.first << this->TerrainData->offset.second;
	}
}

void TerrainObjectClass::DrawTerrainHeight(sf::RenderWindow& Window)
{
	this->OFFSET_HEIGHT_TEXT *= -1;
	for (Draw_Height_Node& Node : TerrainData->HeightMapToDraw)
	{
			                                
		Node.DrawHeight.setPosition(Node.NodePoint.x() + Position.DecPos(0) + TerrainData->offset.first - OFFSET_HEIGHT_TEXT, 
			                        Node.NodePoint.y() + Position.DecPos(1) + TerrainData->offset.second - TerrainData->size.height()+128 + OFFSET_HEIGHT_TEXT);
		Window.draw(Node.DrawHeight);
	}

}

sf::Sprite* TerrainObjectClass::GetSpriteToDraw()
{
	
	TerrainData->Sprite->setPosition(Position.DecPos(0) + TerrainData->offset.first,
		                             Position.DecPos(1) + TerrainData->offset.second - TerrainData->size.height()+128);

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
		TerrainData->GridLines->DrawCell(Window,Number_Cell_Pressed-1);

	}
}

bool TerrainObjectClass::CheckCursorPosition(int x, int y)
{
	QPair<int, int> CoordPointer;
	CoordPointer.first = 0;
	CoordPointer.second = 0;

	QPainterPath path_line;

    FLAG_MOUSE_MOVED = PathContour.contains(QPointF(x, y));
		if (FLAG_MOUSE_MOVED)
		{

			int n = 1;
					for (QPainterPath& Path : CellPathes)
					{
						if (Path.contains(QPointF(x, y)))
							break;
						n++;
						
					}
					this->Number_Cell_Pressed = n;
		}
		return FLAG_MOUSE_MOVED;
}
