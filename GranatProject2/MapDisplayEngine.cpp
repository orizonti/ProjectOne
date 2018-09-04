#include "MapDisplayEngine.h"



MapDisplayEngine::~MapDisplayEngine()
{


}
MapDisplayEngine::MapDisplayEngine()

{
	 Window = new sf::RenderWindow(sf::VideoMode(1800, 1000), "SFML works!");
	 Camera = new sf::View(sf::Vector2f(0, 0), sf::Vector2f(1800, 1000));
	 CellSize = QSize(512, 256);
	Window->setView(*Camera);
	OffsetCamera(0) = 0;
	OffsetCamera(1) = 0;
	WindowSize.setHeight(1000);
	WindowSize.setWidth(1800);

	QString GameDir = qgetenv("GAME_WORK_DIR");
	Font.loadFromFile(GameDir.toStdString() + "/Gc05002t.ttf");

}

void MapDisplayEngine::DrawMap()
{
	Window->clear();
	this->Map.DrawTerrain(*Window);    //DRAW TERRAIN TILES, GRID, AND RED QUADERANGLE OF CURRENT CELL WHEN CURSOR IS MOVING ON HILL CLUSTER
	this->Map.DrawCurrentCell(*Window);//IF CURSOR IS MOVING ON PLAIN TERRAIN OBJECT THEN DRAWING RED RHOMBUS CELL
	this->DrawUnits();


	Window->setView(*Camera);
	Window->display();
}
void MapDisplayEngine::KeyboardControl(sf::Event Keyboard)
{
			if (Keyboard.key.code == sf::Keyboard::Left)
			{
				Camera->move(-CellSize.height(), 0);
				OffsetCamera(0) += 1;
				qDebug() << "Scale - " << Scale << "OffsetCamera(0) - " << OffsetCamera(0);
			}

			if (Keyboard.key.code == sf::Keyboard::Right)
			{
				Camera->move(CellSize.height() , 0);
				OffsetCamera(0) -= 1;
				qDebug() << "Scale - " << Scale << "OffsetCamera(1) - " << OffsetCamera(0);
			}
			if (Keyboard.key.code == sf::Keyboard::Up)
			{
				Camera->move(0, -CellSize.height());
				OffsetCamera(1) += 1;
			}
			if (Keyboard.key.code == sf::Keyboard::Down)
			{
				Camera->move(0, CellSize.height());
				OffsetCamera(1) -= 1;
			}

			if (Keyboard.key.code == sf::Keyboard::S)
			{
				Camera->zoom(2);
				Scale = Scale / 2;

			}
			if (Keyboard.key.code == sf::Keyboard::D)
			{
				Camera->zoom(0.5);
				Scale = Scale * 2;
			}

			if (Keyboard.key.code == sf::Keyboard::G)
			{
				Map.FLAG_DRAW_GRID = true;
			}

			if (Keyboard.key.code == sf::Keyboard::H)
			{
				Map.FLAG_DRAW_GRID = false;
			}

			if (Keyboard.key.code == sf::Keyboard::M)
			{
			}
}

void MapDisplayEngine::MouseControl(sf::Event event)
{
	            //GET POSITION IN GLOBAL MAP COORD SYSTEM 
	            //THAT IS DEFINED AS OFFSET CENTER OF CAMERA ON CELLS COUNT + LENGTH FROM CENTER VIEW TO CURSOR IN CELL MEASUREMENT UNIT

				double x_pos_real;
				double y_pos_real;
		//===============================================================================================
				if (event.type == sf::Event::MouseMoved)
				{
	             x_pos_real = double(event.mouseMove.x - WindowSize.width() / 2) / (CellSize.height()*Scale) - OffsetCamera(0);
				 y_pos_real = double(event.mouseMove.y - WindowSize.height() / 2) / (CellSize.height()*Scale) - OffsetCamera(1);

				MousePosition.SetRealCoord(x_pos_real,y_pos_real);

				Map.MapCellMoved(MousePosition.IsoPos(0), MousePosition.IsoPos(1));
				Map.DefineCellMoved(MousePosition.MousePosReal(0), MousePosition.MousePosReal(1));
				}

		//===============================================================================================
			if (event.type == sf::Event::MouseButtonPressed)
			{
	             x_pos_real = double(event.mouseButton.x - WindowSize.width() / 2) / (CellSize.height()*Scale) - OffsetCamera(0);
				 y_pos_real = double(event.mouseButton.y - WindowSize.height() / 2) / (CellSize.height()*Scale) - OffsetCamera(1);

				MousePosition.SetRealCoord(x_pos_real,y_pos_real);

				Map.MapCellPressed(MousePosition.IsoPos(0), MousePosition.IsoPos(1));

					QPair<int, int> Cell = Map.GetRealCellPressed();
					Units.MapCellPressed(Cell.first,Cell.second);
			}
		//===============================================================================================


}
void MapDisplayEngine::DrawUnits()
{
	Units.DrawUnits(*Window);
}
