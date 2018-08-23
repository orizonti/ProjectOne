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
#include "MapContainerClass.h"

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
			Map.DrawMap();
			clock.restart();
		}
	}

	return 0;
	//return a.exec();
}


