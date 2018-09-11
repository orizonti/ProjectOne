#include "GameDisplayEngine.h"



GameDisplayEngine::~GameDisplayEngine()
{


}
GameDisplayEngine::GameDisplayEngine()

{
	WindowSize.setHeight(800);
	WindowSize.setWidth(1200);

	 Window = new sf::RenderWindow(sf::VideoMode(WindowSize.width(), WindowSize.height()), "SFML works!");
	 Camera = new sf::View(sf::Vector2f(0, 0), sf::Vector2f(WindowSize.width(), WindowSize.height()));
	 CellSize = QSize(512, 256);
	Window->setView(*Camera);
	OffsetCamera.first = 0;
	OffsetCamera.second = 0;

	QString GameDir = qgetenv("GAME_WORK_DIR");
	Font.loadFromFile(GameDir.toStdString() + "/Gc05002t.ttf");

}

void GameDisplayEngine::DrawALL()
{
	Window->clear();
	this->Map.DrawTerrain(*Window);    //DRAW TERRAIN TILES, GRID, AND RED QUADERANGLE OF CURRENT CELL WHEN CURSOR IS MOVING ON HILL CLUSTER
	this->Map.DrawCurrentCell(*Window);//IF CURSOR IS MOVING ON PLAIN TERRAIN OBJECT THEN DRAWING RED RHOMBUS CELL
	Units.DrawUnits(*Window);

	Window->setView(*Camera);
	Window->display();
}
void GameDisplayEngine::KeyboardControl(sf::Event Keyboard)
{
			if (Keyboard.key.code == sf::Keyboard::Left)
			{
				Camera->move(-CellSize.height(), 0);
				OffsetCamera.first += 1;
			}

			if (Keyboard.key.code == sf::Keyboard::Right)
			{
				Camera->move(CellSize.height() , 0);
				OffsetCamera.first -= 1;
			}
			if (Keyboard.key.code == sf::Keyboard::Up)
			{
				Camera->move(0, -CellSize.height());
				OffsetCamera.second += 1;
			}
			if (Keyboard.key.code == sf::Keyboard::Down)
			{
				Camera->move(0, CellSize.height());
				OffsetCamera.second -= 1;
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

}

void GameDisplayEngine::MouseControl(sf::Event event)
{
	            //GET POSITION IN GLOBAL MAP COORD SYSTEM 
	            //THAT IS DEFINED AS OFFSET CENTER OF CAMERA ON CELLS COUNT + LENGTH FROM CENTER VIEW TO CURSOR IN CELL MEASUREMENT UNIT

				double x_pos_real;
				double y_pos_real;
		//===============================================================================================
				if (event.type == sf::Event::MouseMoved)
				{
	             x_pos_real = double(event.mouseMove.x - WindowSize.width() / 2) / (CellSize.height()*Scale) - OffsetCamera.first;
				 y_pos_real = double(event.mouseMove.y - WindowSize.height() / 2) / (CellSize.height()*Scale) - OffsetCamera.second;

				MousePosition.SetRealCoord(x_pos_real,y_pos_real);

				Map.MapCellMoved(MousePosition.IsoPos(0), MousePosition.IsoPos(1));
				Map.DefineCellMoved(MousePosition.MousePosReal(0), MousePosition.MousePosReal(1));
				}

		//===============================================================================================
			if (event.type == sf::Event::MouseButtonPressed)
			{
	             x_pos_real = double(event.mouseButton.x - WindowSize.width() / 2) / (CellSize.height()*Scale) - OffsetCamera.first;
				 y_pos_real = double(event.mouseButton.y - WindowSize.height() / 2) / (CellSize.height()*Scale) - OffsetCamera.second;

				MousePosition.SetRealCoord(x_pos_real,y_pos_real);

				Map.MapCellPressed(MousePosition.IsoPos(0), MousePosition.IsoPos(1));

					QPair<int, int> Cell = Map.GetRealCellPressed();
					Units.MapCellPressed(Cell.first,Cell.second);
			}
		//===============================================================================================


}

void GameDisplayEngine::RunGame()
{

	while (Window->isOpen())
	{
		sf::Event event;
		while (Window->pollEvent(event))
		{
			if(event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseMoved)
			MouseControl(event);

			if (event.type == sf::Event::KeyPressed)
			KeyboardControl(event);
		}

		if (clock.getElapsedTime().asMilliseconds() >= 50)
		{
			Units.MoveUnits();

			DrawALL();
			clock.restart();
		}
	}
}

