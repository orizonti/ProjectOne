#pragma once
#include <HeaderAndStructures.h>

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

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform(); // getTransform() is defined by sf::Transformable
		target.draw(Curve, states);
	}

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

	void DrawPainterPath(int offset_x, int offset_y);

};


class GridShapeContainer
{
public:
	GridShapeContainer()
	{

	}
	void SetOffset(int x, int y);
	void SetPosition(int x, int y);
	void DrawGrid(sf::RenderWindow& Window);


	void AddCurves(QString file);
	QList<CurveShape> Curves;
	QVector<sf::Vector2f> Points;
};
