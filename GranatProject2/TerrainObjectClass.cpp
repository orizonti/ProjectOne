#include "TerrainObjectClass.h"

TerrainObjectClass::TerrainObjectClass(TerrainTileElement* Terrain)
{
	if (Terrain != 0)
	{
		if (Terrain->GridLines != 0)
		{
		PathContour = Terrain->GridLines->GetPathContour();//QPAINTER_PATH TO DEFINE THAT CURRENT TERRAIN IS POINTED BY CURSOR
		CellPathes = Terrain->GridLines->GetSubCells();    //QPAINTER_PATH MASSIVE THAT IS USED TO DEFINE WHAT CELL IS PRESSED ON CURVED TILE
		}
		TerrainData = Terrain;// COMMON TERRAIN TILE ELEMENT
		this->TileSize = TerrainData->Size_By_Cell;
	}

}


void TerrainObjectClass::SetCoord(int x, int y)
{
	if (TerrainData != 0)
	{

		Position.SetCoordIsometric(x, y); //COORDS TO DEFINE THAT UNIT IS MOVING ON THAT TERRAIN in ContainsMapPoint METHOD
		PositionTopRight.SetCoordIsometric(x + TileSize.height(), y + TileSize.width());

		PathContour.translate(Position.DecPos(0) + TerrainData->offset.first, //QPAINTER_PATH TO DEFINE THAT CURRENT TERRAIN IS POINTED BY CURSOR
							  Position.DecPos(1) + TerrainData->offset.second - TerrainData->Size_In_Pixels.height()+128);

	for (QPainterPath& Path : CellPathes)//QPAINTER_PATH MASSIVE THAT IS USED TO DEFINE WHAT CELL IS PRESSED ON CURVED TILE
	Path.translate(Position.DecPos(0) + TerrainData->offset.first, 
		           Position.DecPos(1) + TerrainData->offset.second - TerrainData->Size_In_Pixels.height()+128);
	//ALL CELL CONTOUR IS GETTING REAL DECART POS ON MAP

	}
}


sf::Sprite* TerrainObjectClass::GetSpriteToDraw()
{
	
	TerrainData->Sprite->setPosition(Position.DecPos(0) + TerrainData->offset.first,
		                             Position.DecPos(1) + TerrainData->offset.second - TerrainData->Size_In_Pixels.height()+128);

	return TerrainData->Sprite;
}
//=====================================================================================
void TerrainObjectClass::DrawGrid(sf::RenderWindow& Window)
{

	if (TerrainData->GridLines != 0)
	{
		TerrainData->GridLines->SetPosition(this->Position.DecPos(0) + TerrainData->offset.first, 
			                                this->Position.DecPos(1) + TerrainData->offset.second - TerrainData->Size_In_Pixels.height()+128);
		TerrainData->GridLines->DrawGrid(Window);

		if (FLAG_MOUSE_MOVED)
		TerrainData->GridLines->DrawCell(Window,Number_Cell_Moved-1);

	}
}
//------------------------------------------------------------------------------------
//METHOD IS USED TO DEBUGIN, SHOULD BE DELETED
void TerrainObjectClass::DrawTerrainHeight(sf::RenderWindow& Window)
{
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
			                        Node.NodePoint.y() + Position.DecPos(1) + TerrainData->offset.second - TerrainData->Size_In_Pixels.height()+128 + QuadeOffset[n].y());
		Window.draw(Node.DrawHeight);

		n++;

	}

}
//------------------------------------------------------------------------------------
void TerrainObjectClass::DrawObject(sf::RenderWindow& Window)
{
	Window.draw(*this->GetSpriteToDraw());
}

//=====================================================================================
QPair<int, int> TerrainObjectClass::GetCellPressed()
{
	int x = (Number_Cell_Moved-1) / TileSize.width();
	int y = (Number_Cell_Moved-1) - x*TileSize.width();
	return QPair<int, int>(Position.IsoPos(0) + x,Position.IsoPos(1) + y);
}
QuadeRangleShape& TerrainObjectClass::GetCellBorderMoved()
{
	TerrainData->GridLines->SetPosition(this->Position.DecPos(0) + TerrainData->offset.first, 
		                                this->Position.DecPos(1) + TerrainData->offset.second - TerrainData->Size_In_Pixels.height()+128);

	int x = (Number_Cell_Moved-1) / TileSize.width();
	int y = (Number_Cell_Moved-1) - x*TileSize.width();

	return TerrainData->GridLines->SubCellShapes[Number_Cell_Moved - 1];  
}


bool TerrainObjectClass::ContainsMapPoint(int x, int y)
{
	bool Lager_BL   =         (Position.IsoPos(0) <= x) &&         (Position.IsoPos(1) <= y);
	bool Smaller_TR = (PositionTopRight.IsoPos(0) > x) && (PositionTopRight.IsoPos(1) > y);

	return (Lager_BL && Smaller_TR);
}

QVector<double>&  TerrainObjectClass::GetHeightMapOnCell(int x, int y)
{
	int x_relative = x - Position.IsoPos(0);
	int y_relative = y - Position.IsoPos(1);

		int n = y_relative*TileSize.height() + x_relative;

		if (n >= 0)
			return TerrainData->HeightMap[n];
		else
			return QVector<double>();

}


bool TerrainObjectClass::CheckCursorPosition(int x, int y)
{
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
