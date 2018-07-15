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
#include "MapConteinerClass.h"

class PathPoints
{
public:
	QString typePath = "none";
	QList<QPointF> Points;
};



class CurveShape : public sf::Shape
{
public:

	void AddCurves(QDomElement newElement);

	explicit CurveShape()
	{

		Curve.setPrimitiveType(sf::LineStrip);
	}

	QList<PathPoints> PathMassive;
	QPainterPath Path;
	QList<QPolygonF> listPoints;

	sf::VertexArray Curve;


	virtual std::size_t getPointCount() const
	{
		return listPoints.at(0).size(); // fixed, but could be an attribute of the class if needed
	}

	virtual sf::Vector2f getPoint(std::size_t index) const
	{

		//qDebug() << "index - " << index;
		sf::Vector2f Coord;
		Coord.x = listPoints.at(0).at(index).x();
		Coord.y = listPoints.at(0).at(index).y();
		return Coord;
	}

	QPointF GetCoord(QString &str, QRegExp &rx)
	{
		QPointF Point;
		rx.indexIn(str);
		Point.setX(rx.cap(1).toDouble()); // "189"
		Point.setY(rx.cap(2).toDouble()); // "189"
		return Point;
	}
private:
	void DrawPainterPath();

};


class SVGCurveContainer
{
public:
	SVGCurveContainer(QString path = "d:/WorkDir/444.svg")
	{
		QFile newXMLFile(path);
		bool result = newXMLFile.open(QIODevice::ReadOnly);

		QDomDocument newDomDoc;
		result = newDomDoc.setContent(&newXMLFile);



		if (result)
		{
			QDomNode currentNod = newDomDoc.documentElement().firstChild();
			while (!currentNod.isNull())
			{
				QDomElement newElement = currentNod.toElement();

				qDebug() << "*************************************";
				if (newElement.tagName() == "path")
				{
					CurveShape newShape;
					newShape.AddCurves(newElement);
					Curves.append(newShape);
				}

				currentNod = currentNod.nextSibling();
			}
		}


	}
	QList<CurveShape> Curves;
};





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
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	sf::View view2(sf::Vector2f(640, 320), sf::Vector2f(1280, 640));

	sf::Image img;
	sf::Image border;

	bool res = img.loadFromFile("E:/WorkDir/WORK_DIR/TERRAIN_ISOMETRIC_TILES/Tiles/with_grid/GrassTest.png");
//	bool res = img.loadFromFile("E:/WorkDir/2dworkdir/Terrain/1x1Green.png");
	res = border.loadFromFile("E:/WorkDir/2dworkdir/Terrain/1x1Border.png");


	qDebug() << "Curretn path - " << QDir::currentPath();

	sf::Texture texture;
	sf::Texture textborder;

	texture.loadFromImage(img);

	textborder.loadFromImage(border);

	sf::Sprite sprite;
	sf::Sprite spriteBorder;
	sprite.setTexture(texture);

	spriteBorder.setTexture(textborder);
	spriteBorder.setPosition(0, 320);


	// Matrix from isometric to decarc
	Eigen::Matrix2d m;
	Eigen::Matrix2d n;
	m << 1, 1,
		-0.5, 0.5;

	n << 0.5, -1,
		0.5, 1;

	Eigen::Vector2d IsoVect;
	Eigen::Vector2d IsoVect2;
	Eigen::Vector2d DecVect;
	Eigen::Vector2d DecVect2;



	sf::Clock clock; // starts the clock
	double Period;
	double x_center = 0;
	double y_center = 0;


	//ClassHero Hero;
	//CurveShape Shape;

	MapConteinerClass Map;
	Map.CreateMapFromFile(QString("E:/WorkDir/WORK_DIR/MAPS_TILED/TestMapBig.tmx"));

	SVGCurveContainer Contaner;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseMoved)
			{

				DecVect2(0) = double(event.mouseMove.x) / 64.0 + x_center / 64;
				DecVect2(1) = -(double(event.mouseMove.y) - 350.0) / 64.0 + y_center / 64;

				IsoVect2 = n*DecVect2;

				IsoVect2(0) = floor(IsoVect2(0));
				IsoVect2(1) = floor(IsoVect2(1));

				DecVect2 = m*IsoVect2 * 64;
				DecVect2(1) = 320 - DecVect2(1);
				//qDebug() << "IsoVect - " << IsoVect2(0) << IsoVect2(1) << "DecVect - " << DecVect2(0) << DecVect2(1);


			}

			if (event.type == sf::Event::MouseButtonPressed)
			{

				DecVect2(0) = double(event.mouseMove.x) / 64.0 + x_center / 64;
				DecVect2(1) = -(double(event.mouseMove.y) - 350.0) / 64.0 + y_center / 64;

				IsoVect2 = n*DecVect2;

				IsoVect2(0) = floor(IsoVect2(0));
				IsoVect2(1) = floor(IsoVect2(1));


				///Hero.SetDestination(IsoVect2(0), IsoVect2(1));

			}
		}



		if (clock.getElapsedTime().asMilliseconds() >= 300)
		{
			clock.restart();

			window.clear();

//			window.draw(sprite);
			Map.DrawMap(window);

			//Hero.MoveHero();

			//spriteBorder.setPosition(DecVect2(0), DecVect2(1));
			//window.draw(spriteBorder);
			//window.draw(Hero.ImageHero.spriteHero);
			//for (CurveShape Shape : Contaner.Curves)
			//	window.draw(Shape.Curve);


			window.setView(view2);
			window.display();
		}



	}

	return 0;
	//return a.exec();
}


void CurveShape::DrawPainterPath()
{

	for (PathPoints sub_path : this->PathMassive)
	{
		if (sub_path.typePath == "MOVE")
			Path.moveTo(sub_path.Points.at(0));

		if (sub_path.typePath == "LINE")
			Path.lineTo(sub_path.Points.at(0));

		if (sub_path.typePath == "CUBIC")
			Path.cubicTo(sub_path.Points.at(0), sub_path.Points.at(1), sub_path.Points.at(2));
	}


	listPoints = Path.toSubpathPolygons();

	for (QPointF point : listPoints.at(0))
	{

		sf::Vertex vertex;
		vertex.position = sf::Vector2f(point.x(), point.y());
		vertex.color = sf::Color::Green;
		Curve.append(vertex);

	}
	qDebug() << "Nuber polygons - " << listPoints.at(0).size();
}


void CurveShape::AddCurves(QDomElement newElement)
{


	QString curve_path_str = newElement.attribute("d");

	QStringList list;
	list = curve_path_str.split(QRegExp("\\s"));

	QRegExp rx("(\\d{1,4}.\\d{3}),(\\d{1,4}.\\d{3})");

	QString str;
	QPointF Point;
	for (int n = 0; n < list.size(); n++)
	{
		str = list.at(n);
		PathPoints newPath;

		if (str[0] == 'M')
		{
			str.remove(0, 1);
			Point = GetCoord(str, rx);
			newPath.typePath = "MOVE";
			newPath.Points.append(Point);

			this->PathMassive.append(newPath);
			qDebug() << "MOVE TO - " << Point;
		}

		if (str[0] == 'L')
		{
			str.remove(0, 1);
			Point = GetCoord(str, rx);
			newPath.typePath = "LINE";
			newPath.Points.append(Point);
			this->PathMassive.append(newPath);
			qDebug() << "LINE TO - " << Point;
		}

		if (str[0] == 'C')
		{

			str.remove(0, 1);
			Point = GetCoord(str, rx);
			newPath.Points.append(Point);

			n++;
			str = list.at(n);
			Point = GetCoord(str, rx);
			newPath.Points.append(Point);

			n++;
			str = list.at(n);
			Point = GetCoord(str, rx);
			newPath.Points.append(Point);


			newPath.typePath = "CUBIC";
			this->PathMassive.append(newPath);
			qDebug() << "CUBIC BEZIER - " << newPath.Points.at(0) << newPath.Points.at(1) << newPath.Points.at(2);
		}

	}

	DrawPainterPath();
	update();
}