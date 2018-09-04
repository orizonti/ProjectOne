#include <QtCore/QCoreApplication>
#include <QtCore/QCoreApplication>
#include <QDebug>
#include <QDir>
#include "SFML\Graphics.hpp"
#include <Eigen/Dense>
#include <iostream>
#include <qstring>
#include "QPainterPath"
#include "QPolygon"
#include "qfile.h"
#include "QtXml\qxml.h"
#include "QtXml\qdom.h"
#include "MapDisplayEngine.h"

QSize GameCoord::CellSize = QSize(512, 256);
QString GameDir = "";

void operator >> (sf::Event event, EventControlInterface& Interface)
{
	if(event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseMoved)
	Interface.MouseControl(event);

	if (event.type == sf::Event::KeyPressed)
	Interface.KeyboardControl(event);
}

int main(int argc, char *argv[])
{
	//QCoreApplication a(argc, argv);

	GameCoord Coord1;
	GameCoord Coord2;
	GameCoord Coord3;
	Coord1.SetCoordIsometric(2, 4);
	Coord2.SetCoordIsometric(2, 4);
	Coord3.SetCoordIsometric(2, 3);

	Coord3.translate(0, 0.1);
	Coord3.translate(0, 0.1);
	Coord3.translate(0, 0.1);
	Coord3.translate(0, 0.1);
	Coord3.translate(0, 0.1);
	Coord3.translate(0, 0.1);
	Coord3.translate(0, 0.1);
	Coord3.translate(0, 0.1);
	Coord3.translate(0, 0.1);
	Coord3.translate(0, 0.1);
	//qDebug() << "COORD 3 - " << Coord3.GetIsoCoord();

	//qDebug() << "COORD1 == COORD2 - " << (Coord1 == Coord2);
	//qDebug() << "COORD1 == COORD3 - " << (Coord1 == Coord3);

	double x = 1.1;
	double x1 = 0.1;
	x1 = x1 + double(0.1);
	x1 = x1 + double(0.1);
	x1 = x1 + double(0.1);
	x1 = x1 + double(0.1);
	x1 = x1 + double(0.1);

	x1 = x1 + double(0.1);
	x1 = x1 + double(0.1);
	x1 = x1 + double(0.1);
	x1 = x1 + double(0.1);
	x1 = x1 + double(0.1);

	//qDebug() << "Test == " << (std::floor(10*x) == std::floor(10*x1));

	GameDir = qgetenv("GAME_WORK_DIR");

	sf::Clock clock; // starts the clock
	MapDisplayEngine Map;



	while (Map.Window->isOpen())
	{
		sf::Event event;
		while (Map.Window->pollEvent(event))
		{
			event >> Map;
		}

		if (clock.getElapsedTime().asMilliseconds() >= 50)
		{
			Map.Units.MoveUnits();
			Map.DrawMap();
			clock.restart();
		}
	}

	return 0;
	//return a.exec();
}


