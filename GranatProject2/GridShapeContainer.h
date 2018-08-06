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
	void AddCurves(QPainterPath pathShape);

	explicit CurveShape()
	{

		Curve.setPrimitiveType(sf::TrianglesStrip);
	}
	void SetColor(sf::Color color);

	QList<PathPoints> PathMassive;
	QPainterPath Path;
	QList<QPolygonF> listPoints;
	QVector<QPainterPath> SubPathEdge;
	int Direction = 0;
	sf::VertexArray Curve;

	void PainterPathToShape(QPainterPath path);

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

	void operator=(const CurveShape& Shape)
	{
	 Path.addPath(Shape.Path);
	 listPoints.append(Shape.listPoints);
	 SubPathEdge.append(Shape.SubPathEdge);
	 Direction = Shape.Direction;

	 for(int n = 0; n < Shape.Curve.getVertexCount(); n++)
	 Curve.append(Shape.Curve[n]);

	 PathMassive.append(Shape.PathMassive);
	}

	CurveShape& GetCopy()
	{
		CurveShape* NewShape = new CurveShape;
		*NewShape = *this;
		return *NewShape;
	}

};

class QuadeRangleShape
{
public:
	void AddCurves(QVector<QPainterPath> EdgePaths);
	void SetColor(sf::Color color);
	QVector<CurveShape> EdgeShapes;
	QPainterPath ClosePath;
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

	void DrawContour(sf::RenderWindow& Window);


	void AddCurves(QString file);
	QMap<int,QVector<QPainterPath>> GetStripePathes();
	QPainterPath GetPathContour();


	QList<CurveShape> CurvesVert;
	QList<CurveShape> CurvesHoriz;

	QList<QVector<CurveShape>> CurveStripeHoriz;
	QList<QVector<CurveShape>> CurveStripeVert;

	QVector<CurveShape> ContourShapes;

	QVector<sf::Vector2f> Points;
};
