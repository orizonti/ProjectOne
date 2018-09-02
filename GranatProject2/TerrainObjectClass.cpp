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
	QVector<QPoint> QuadeOffset;

	QPoint OffsetPoint;  OffsetPoint.setX(-30); OffsetPoint.setY(-15);
	QPoint OffsetPoint2; OffsetPoint2.setX(0); OffsetPoint2.setY(-30);
	QPoint OffsetPoint3; OffsetPoint3.setX(0); OffsetPoint3.setY(0);
	QPoint OffsetPoint4; OffsetPoint4.setX(30); OffsetPoint4.setY(-15);
	QuadeOffset.append(OffsetPoint);
	QuadeOffset.append(OffsetPoint2);
	QuadeOffset.append(OffsetPoint3);
	QuadeOffset.append(OffsetPoint4);

	int n = 0;
	for (Draw_Height_Node& Node : TerrainData->HeightMapToDraw)
	{
		if (n == 4)
			n = 0;

		Node.DrawHeight.setPosition(Node.NodePoint.x() + Position.DecPos(0) + TerrainData->offset.first  - QuadeOffset[n].x(), 
			                        Node.NodePoint.y() + Position.DecPos(1) + TerrainData->offset.second - TerrainData->size.height()+128 + QuadeOffset[n].y());
		Window.draw(Node.DrawHeight);

		n++;

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

QuadeRangleShape& TerrainObjectClass::GetCellBorderMoved()
{
	TerrainData->GridLines->SetPosition(this->Position.DecPos(0) + TerrainData->offset.first, 
		                                this->Position.DecPos(1) + TerrainData->offset.second - TerrainData->size.height()+128);
	return TerrainData->GridLines->SubCellShapes[Number_Cell_Moved - 1];  
}

void TerrainObjectClass::DrawGrid(sf::RenderWindow& Window)
{

	if (TerrainData->GridLines != 0)
	{
		TerrainData->GridLines->SetPosition(this->Position.DecPos(0) + TerrainData->offset.first, 
			                                this->Position.DecPos(1) + TerrainData->offset.second - TerrainData->size.height()+128);
		TerrainData->GridLines->DrawGrid(Window);

		if (FLAG_MOUSE_MOVED)
		TerrainData->GridLines->DrawCell(Window,Number_Cell_Moved-1);

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
					this->Number_Cell_Moved = n;
		}
		return FLAG_MOUSE_MOVED;
}
