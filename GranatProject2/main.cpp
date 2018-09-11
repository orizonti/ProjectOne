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
#include "GameDisplayEngine.h"

QSize GameCoord::CellSize = QSize(512, 256);
QString GameDir = "";


int main(int argc, char *argv[])
{
	//QCoreApplication a(argc, argv);

	GameDir = qgetenv("GAME_WORK_DIR");

	GameDisplayEngine GameEngine;
					  GameEngine.RunGame();
	                 

	return 0;
	//return a.exec();
}


