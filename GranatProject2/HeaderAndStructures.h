#pragma once
#include "QString"
#include "SFML\Graphics.hpp"
#include "qfile.h"
#include "QtXml\qxml.h"
#include "QtXml\qdom.h"
#include "QDebug"
#include "QSize"
#include "QPainterPath"
#include <iostream>
#include <qstring>
#include "QPolygon"
#include <Eigen/Dense>
#include <qstring>
#include <QDebug>
#include <QDir>
#include <math.h>

extern QString GameDir;
class EventControlInterface
{
public:
	virtual void KeyboardControl(sf::Event Key_event) = 0;
	virtual void MouseControl(sf::Event Mouse_event) = 0;

	friend void operator >> (sf::Event event, EventControlInterface& Interface);
};


