#pragma once
#include <HeaderAndStructures.h>
#include <GameCoord.h>
#include <MapContainerClass.h>

class MapDisplayEngine :public EventControlInterface
{
public: 
	MapDisplayEngine();
	~MapDisplayEngine();

	 sf::RenderWindow* Window;
	 sf::View* Camera;

	 float Scale = 1;
	 QSize WindowSize;
	 QSize CellSize;

	Eigen::Vector2d OffsetCamera;

    GameCoord MousePosition;

	 void KeyboardControl(sf::Event event);
	 void MouseControl(sf::Event event);


	 void DrawMap();
	 void DrawUnits();
	 
	 MapContainerClass Map;
	 GameViewUnitContainer Units;
};

