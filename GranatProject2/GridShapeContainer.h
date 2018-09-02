#pragma once
#include <HeaderAndStructures.h>
#include <GameCoord.h>

class PathPoints
{
public:
	QString typePath = "none";
	QList<QPointF> Points;
};



class CurveShape : public sf::Shape
{
public:

	CurveShape(QPainterPath& path, int Dir);
	explicit CurveShape();

	void operator=(const CurveShape& Shape);
	CurveShape& GetCopy();

	void AddCurves(QPainterPath pathShape,int Dir);
	void AddLine(sf::Vector2f Start, sf::Vector2f End, int Dir);
	void CreateCurvesFromSVG(QDomElement  newElement,int Dir);
	void SetColor(sf::Color color);


	virtual void         draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual std::size_t  getPointCount() const;
	virtual sf::Vector2f getPoint(std::size_t index) const;


	QVector<QPointF>       NodePoints; //POINTS IS USED TO GENERATE HEIGHT MAP OF TERRAIN, IT IS GETTING WHEN CURVES IS CREATING FROM SVG FILE, ELSE NodePoints IS EMPTY
	QVector<QPainterPath>  SubPathEdge;//USED TO COMPOSE QUADRANGLE
	QPainterPath           Path       ;
private:
	void CreateCurvesFromPathes(int offset_x, int offset_y);
	void PainterPathToShape(QPainterPath path);

	QPointF GetCoord(QString &str, QRegExp &rx);// UTILITY TO PARSING SVG FILE

	int Direction = 0; //USED TO PROPERLY CREATE LINE WITH THICKNESS  
	QList<PathPoints>      PathMassive;
	QList<QPolygonF>       listPoints ;
	sf::VertexArray        Curve; //USED TO DRAW SHAPE BY VERTEX DOTS
};

class QuadeRangleShape
{
public:
    QuadeRangleShape(QVector<CurveShape> Shapes);
	QuadeRangleShape(const QuadeRangleShape& Shape);
    explicit QuadeRangleShape();
	void SetQuadeShapes(QVector<CurveShape> Shapes);

	void SetPosition(int x, int y);
	void SetColor(sf::Color color);

	void DrawShape(sf::RenderWindow& Window);
private:
	QVector<CurveShape> EdgeShapes;
};

class Draw_Height_Node
{
public:
	Draw_Height_Node()
	{
	DrawHeight.setFont(Font); // font is a sf::Font
	DrawHeight.setFillColor(sf::Color::Red);
	DrawHeight.setStyle(sf::Text::Bold | sf::Text::Underlined);
	DrawHeight.setCharacterSize(18); // in pixels, not points!
	}


	QPointF NodePoint;
	sf::Text DrawHeight;
};

class GridShapeContainer
{
public:
	GridShapeContainer();
	void AddCurves(QString path);

	void DrawGrid(sf::RenderWindow& Window);
	void DrawCell(sf::RenderWindow& Window, int NumberCell);
	void DrawContour(sf::RenderWindow& Window);

	void DrawTerrainHeight(sf::RenderWindow& Window);
	void SetOffset(int x, int y);
	void SetPosition(int x, int y);

	QVector<QPainterPath> GetSubCells();
	QPainterPath          GetPathContour();


	QList<CurveShape>         CurvesVert; //LINES
	QList<CurveShape>         CurvesHoriz;
	QVector<QuadeRangleShape> SubCellShapes;//QUADERANGLES
private:
	QVector<QPainterPath>     SubCellPathes;
	QVector<CurveShape>       ContourShapes;

	QVector<sf::Vector2f>     Points; //LINE'S POINTS TO DRAWING
};
