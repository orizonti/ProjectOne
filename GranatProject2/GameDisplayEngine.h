#pragma once
#include <HeaderAndStructures.h>
#include <GameCoord.h>
#include <MapContainerClass.h>

class GameDisplayEngine :public EventControlInterface
{
public: 
	GameDisplayEngine();
	~GameDisplayEngine();

	 sf::RenderWindow* Window;
	 sf::View* Camera;

	 float Scale = 1;
	 QSize WindowSize;
	 QSize CellSize;
	 QPair<int,int> OffsetCamera; //POSITION OF CAMERA VIEW ON MAP IN CELL COUNT

	sf::Clock clock; // 

    GameCoord MousePosition;  

	 void KeyboardControl(sf::Event event);
	 void MouseControl(sf::Event event);
	 void RunGame();


	 void DrawALL();
	 
	 MapContainerClass Map;
	 GameViewUnitContainer Units;

};

