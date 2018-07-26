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






class DisplayMapClass
{
public:
	DisplayMapClass()
	{

	}
	//GameImage MapTile;

};




int main(int argc, char *argv[])
{
	//QCoreApplication a(argc, argv);
	sf::RenderWindow window(sf::VideoMode(1280, 640), "SFML works!");

	sf::View view2(sf::Vector2f(640, 320), sf::Vector2f(1280, 640));



	std::string GameDir = qgetenv("GAME_WORK_DIR");


	sf::Clock clock; // starts the clock
	double Period;

	MapContainerClass Map;
	Map.CreateMapFromFile(QString("E:/WorkDir/WORK_DIR/MAPS_TILED/TestMapBig.tmx"));


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{

		}



		if (clock.getElapsedTime().asMilliseconds() >= 50)
		{
			clock.restart();
		}



	}

	return 0;
	//return a.exec();
}


