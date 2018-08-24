#include <GridShapeContainer.h>

GridShapeContainer::GridShapeContainer()
{

}
void GridShapeContainer::AddCurves(QString path)
{
	    QString GameDir = qgetenv("GAME_WORK_DIR");
		QFile newXMLFile(GameDir + "/WORK_DIR/TERRAIN_ISOMETRIC_TILES/PNG HILL TILES/TilesHill/" + path);
		bool result = newXMLFile.open(QIODevice::ReadOnly);
		QDomDocument newDomDoc;
		result = newDomDoc.setContent(&newXMLFile);



		QDomNodeList GroupsNodes = newDomDoc.documentElement().firstChild().nextSibling().toElement().elementsByTagName("g");


    //--------------------------------------------------------------------------------------------
        //DOWNLOAD SVG GRID LINES
		for (int direction = 0; direction <= 1; direction++)
		{

			QDomNode currentNod = GroupsNodes.at(direction).firstChild();

			while (!currentNod.isNull())
			{
				QDomElement newElement = currentNod.toElement();

				if (newElement.tagName() == "path")
				{
					CurveShape newShape;
					newShape.CreateCurvesFromSVG(newElement,direction); // PARSING SVG FILE AND CREATING SHAPE TO DRAW


					if(direction == 0)
					CurvesHoriz.append(newShape);
					if(direction == 1)
					CurvesVert.append(newShape);
				}

				currentNod = currentNod.nextSibling();
			}
		}
		
		
    //--------------------------------------------------------------------------------------------
    //CREATING MASSIVE OF CELL QUADRANGLE IS TO DEFINE AND HIGHLIGHT CURRENT CELL IN HILL 
	for (int y = 0; y < CurvesHoriz.size()-1; y++)
	{
		for (int x = 0; x < CurvesVert.size()-1; x++)
		{

				QPainterPath PathBottom = CurvesHoriz[y].SubPathEdge[x+1].toReversed();
				QPainterPath PathUp =      CurvesHoriz[y+1].SubPathEdge[x+1];

				QPainterPath PathRibLeft = CurvesVert[x].SubPathEdge[y+1];
				QPainterPath PathRibRight = CurvesVert[x+1].SubPathEdge[y+1].toReversed();

					PathRibLeft.connectPath(PathUp);       //CREATING LOOP PAINTER PATH ,THAT IS USED TO DIFINE ,THAT CURSOS POINT IS IN THE CELL BOUND
					PathRibLeft.connectPath(PathRibRight);
					PathRibLeft.connectPath(PathBottom);
					PathRibLeft.closeSubpath();
					SubCellPathes.append(PathRibLeft);
                //---------------------------------------------------------------------------------
				QVector<CurveShape> QuadShapes;           //QUADRANGLE SHAPES MASSIVE IS TO HIGHLIGHT CURRENT CELL, WHEN CURSOR MOVING ON IT
				QuadShapes.append(CurveShape(PathBottom,0));
				QuadShapes.append(CurveShape(PathUp,0));
				QuadShapes.append(CurveShape(PathRibLeft,1));
				QuadShapes.append(CurveShape(PathRibRight,1));

				QuadeRangleShape QuadRangle(QuadShapes);
				this->SubCellShapes.append(QuadRangle);
                //---------------------------------------------------------------------------------


		}

	}
    //--------------------------------------------------------------------------------------------
}

CurveShape::CurveShape(QPainterPath& path,int Dir)
{
	//TRIANGLE STRIP TYPE ALLOW TO GET LINES WITH THICKNESS
	Curve.setPrimitiveType(sf::TrianglesStrip);
	AddCurves(path,Dir);
}

CurveShape::CurveShape()
{
	Curve.setPrimitiveType(sf::TrianglesStrip);
}
void CurveShape::CreateCurvesFromPathes(int offset_x = 0, int offset_y = 0)
{
	QPointF LastPoint;
	for (PathPoints sub_path : this->PathMassive)   //QPAINTER_PATH ALLOW TO PERFORM BEZIER OPERATIONS THAT CONTAINS IN SVG CURVE FILE, AND TO GET CURVE POINTS TO CREATE DRAWABLE SF::CURVE
	{
	QPainterPath sub_path_rib;
		if (sub_path.typePath == "MOVE")
		{
			Path.moveTo(sub_path.Points.at(0));

			LastPoint = sub_path.Points.at(0);
		}

		if (sub_path.typePath == "LINE")
		{
			Path.lineTo(sub_path.Points.at(0));    //CREATING WHOLE LINE OF TERRAIN GRID

			sub_path_rib.moveTo(LastPoint);        //CREATING SUB ELEMENT OF LINE IS TO LINK IT TO QUADRANGLE LATER 
			sub_path_rib.lineTo(sub_path.Points.at(0));

			LastPoint = sub_path.Points.at(0);
		}

		if (sub_path.typePath == "CUBIC")
		{
			Path.cubicTo(sub_path.Points.at(0), sub_path.Points.at(1), sub_path.Points.at(2));

			sub_path_rib.moveTo(LastPoint);
			sub_path_rib.cubicTo(sub_path.Points.at(0), sub_path.Points.at(1), sub_path.Points.at(2));

			LastPoint = sub_path.Points.at(2);
     	}
	SubPathEdge.append(sub_path_rib);
	}

	PainterPathToShape(Path); // CREATING VERTAX ARRAY OF SHAPE THAT IS USED TO DRAW GRID IN HILL

}
void CurveShape::AddCurves(QPainterPath pathShape, int Dir)
{
	Direction = Dir;
	if (!pathShape.toSubpathPolygons().isEmpty())
	PainterPathToShape(pathShape);

}


void CurveShape::SetColor(sf::Color color)
{

	for (int n = 0; n < Curve.getVertexCount(); n++)
	{
		Curve[n].color = color;
	}

}

void CurveShape::PainterPathToShape(QPainterPath path)
{

	// CREATING VERTAX ARRAY OF SHAPE THAT IS USED TO DRAW GRID IN HILL

	listPoints = path.toSubpathPolygons(); //GET POINTS FROM PAINTER PATH CURVE

	int x_offset = 8;
	int y_offset = -4;

	if (this->Direction == 1)
		y_offset = 4;

	for (QPointF point : listPoints.at(0))
	{
        //CREATE TWO VERTEX POINTS FROM PAINTER PATH QPOINT
		//TWO POINT IS NEEDED TO GET LINE WITH THICKNES AS COMBINTAION OF TRIANGLE, SEE sf::TriangleStrip TYPE
		sf::Vertex vertex;
		vertex.position = sf::Vector2f(point.x(), point.y());
		vertex.color = sf::Color::Black;
		Curve.append(vertex);

		sf::Vertex vertex2;
		vertex2.position = sf::Vector2f(point.x() + x_offset, point.y() + y_offset);
		vertex2.color = sf::Color::Black;
		Curve.append(vertex2);
	}
}

void CurveShape::AddLine(sf::Vector2f Start, sf::Vector2f End, int Dir)
{
	//CREATING LINE SHAPE AS COMBINATION OF FOUR POINTS
    //TWO POINT AT EDGE IS NEEDED TO GET LINE WITH THICKNES AS COMBINTAION OF TRIANGLE, SEE sf::TriangleStrip TYPE
	this->Direction = Dir;

	int x_offset = 8;
	int y_offset = -4;

	if (this->Direction == 1)
		y_offset = 4;


		sf::Vertex vertex;
		vertex.position = sf::Vector2f(Start.x, Start.y);
		vertex.color = sf::Color::Black;
		Curve.append(vertex);

		sf::Vertex vertex2;
		vertex2.position = sf::Vector2f(Start.x + x_offset, Start.y + y_offset);
		vertex2.color = sf::Color::Black;
		Curve.append(vertex2);

		sf::Vertex vertex3;
		vertex3.position = sf::Vector2f(End.x, End.y);
		vertex3.color = sf::Color::Black;
		Curve.append(vertex3);

		sf::Vertex vertex4;
		vertex4.position = sf::Vector2f(End.x + x_offset, End.y + y_offset);
		vertex4.color = sf::Color::Black;
		Curve.append(vertex4);
}

void CurveShape::CreateCurvesFromSVG(QDomElement newElement,int Dir)
{
	//PARSING SVG CURVE FILE TO GET MASSIVE OF BEZIER PATH ELEMENTS, THAT LATER IS USED TO GENERATE PAINTER PATH AND VERTEX ARRAY
	this->Direction = Dir;

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
		}

		if (str[0] == 'L')
		{
			str.remove(0, 1);
			Point = GetCoord(str, rx);
			newPath.typePath = "LINE";
			newPath.Points.append(Point);
			this->PathMassive.append(newPath);
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
		}
		    if(!newPath.Points.isEmpty())
			NodePoints.append(newPath.Points.last()); // GETTING NODE POINTS TO GENERATE HEIGTH MAP OF TERRAIN

	}
	CreateCurvesFromPathes();
}


QVector<QVector<double>> GridShapeContainer::GetHeightMap()
{
	GameCoord LineCoord;
	QVector<QVector<double>> QuadeHeights;
	QVector<QVector<double>> LineHeights;


	int iso_pos_y = 0;
	int x_cathetus = 0;
	int y_cathetus = 0;
	QPair<int,int> ShapePoint;
	for (CurveShape& Shape : CurvesVert)
	{
		QVector<double> LineOfDots;
		for (QPointF& Point : Shape.NodePoints)
		{
			LineCoord.SetCoordIsometric(0, iso_pos_y);
			ShapePoint = LineCoord.GetDecCoord();
			x_cathetus = Point.x() - ShapePoint.first;
			y_cathetus = Point.y() - ShapePoint.second;
			double Height = std::hypot(x_cathetus, y_cathetus);
			LineOfDots.append(Height);
			iso_pos_y++;
		}
		LineHeights.append(LineOfDots);

	}

	return QuadeHeights;
}

void GridShapeContainer::SetOffset(int x, int y)
{
	//ALL SHAPES IS CREATED AT BEGINING OF MAP COORD SYSTEM, BEFORE DRAWING SetOffset or SetPosition MUST BE INVOKED TO DRAW IT IN PROPER PLACE
	for (CurveShape& Shape : this->CurvesVert)
		Shape.move(x, y);
	for (CurveShape& Shape : this->CurvesHoriz)
		Shape.move(x, y);

	for(CurveShape& Shape : this->ContourShapes)
		Shape.move(x, y);

}

void GridShapeContainer::SetPosition(int x, int y)
{
	//ALL SHAPES IS CREATED AT BEGINING OF MAP COORD SYSTEM, BEFORE DRAWING SetOffset or SetPosition MUST BE INVOKED TO DRAW IT IN PROPER PLACE
	for (CurveShape& Shape : this->CurvesVert)
		Shape.setPosition(x, y);
	for (CurveShape& Shape : this->CurvesHoriz)
		Shape.setPosition(x, y);

	for(CurveShape& Shape : this->ContourShapes)
		Shape.setPosition(x, y);

	for(QuadeRangleShape& Shape : this->SubCellShapes)
		Shape.SetPosition(x, y);

}

void GridShapeContainer::DrawContour(sf::RenderWindow& Window)
{
	for(CurveShape& Shape : this->ContourShapes)   // DrawContour IS INVOKED WHEN CURSOR ON THE CURRENT HILL
		Window.draw(Shape);
}

void GridShapeContainer::DrawCell(sf::RenderWindow& Window, int NumberCell)
{
	this->SubCellShapes[NumberCell].SetColor(sf::Color::Red);   //DRAW CELL IS INVOKED TO HIGHLING HILL'S CELL THAT IS POINTED BY CURSOR
	this->SubCellShapes[NumberCell].DrawShape(Window);
	this->SubCellShapes[NumberCell].SetColor(sf::Color::Black);
	
}

void GridShapeContainer::DrawGrid(sf::RenderWindow& Window)
{

//	if (CurvesVert.isEmpty())
//		return;

//	for (CurveShape Shape : this->CurvesVert)
//		Window.draw(Shape);

//	for (CurveShape Shape : this->CurvesHoriz)
//		Window.draw(Shape);
//	if (SubCellShapes.size() == 0)
//		return;

	for (QuadeRangleShape& QuadeShape : this->SubCellShapes)
		QuadeShape.DrawShape(Window);


}

QPainterPath GridShapeContainer::GetPathContour()
{
	//COMPOSING CONTOUR LOOP PATH FROM BORDER LINES, THAT IS USED TO DEFINE THAT TERRAIN ELEMENT IS POINTED BY CURSOR
	QPainterPath PathUp;
	QPainterPath PathBottom;
	QPainterPath PathRibLeft;
	QPainterPath PathRibRight;

	if (CurvesVert.isEmpty())
		return PathRibLeft;
		//=======================================================
	PathBottom = CurvesHoriz.first().Path.toReversed();
	PathUp = CurvesHoriz.last().Path;
	PathRibLeft = CurvesVert.first().Path;
	PathRibRight = CurvesVert.last().Path.toReversed();

    //GET CONTOUR SHAPES TO HIGHLIGHT CURRENT TERRAIN ELEMENT
	ContourShapes.append(CurvesHoriz.first().GetCopy());
	ContourShapes.append(CurvesHoriz.last().GetCopy());
	ContourShapes.append( CurvesVert.first().GetCopy());
	ContourShapes.append( CurvesVert.last().GetCopy());

	for(auto Shape: ContourShapes)
	Shape.SetColor(sf::Color::Red);


	    //CREATING CLOSE LOOP PAINTER PATH THAT ALLOW TO DEFINE POINT CONTAINING
		PathRibLeft.connectPath(PathUp);
		PathRibLeft.connectPath(PathRibRight);
		PathRibLeft.connectPath(PathBottom);
		PathRibLeft.closeSubpath();
		return PathRibLeft;
}

QVector<QPainterPath> GridShapeContainer::GetSubCells()
{
		return SubCellPathes;
}

void QuadeRangleShape::SetQuadeShapes(QVector<CurveShape> Shapes)
{
	this->EdgeShapes = Shapes;
}

QuadeRangleShape::QuadeRangleShape(QVector<CurveShape> Shapes)
{
	this->EdgeShapes = Shapes;
}

QuadeRangleShape::QuadeRangleShape()
{
}

void QuadeRangleShape::SetColor(sf::Color color)
{

	for (CurveShape& Shape : EdgeShapes)
		Shape.SetColor(color);
}

void QuadeRangleShape::SetPosition(int x, int y)
{
	for (CurveShape& Shape : EdgeShapes)
		Shape.setPosition(x, y);

}

void QuadeRangleShape::DrawShape(sf::RenderWindow& Window)
{
	if (EdgeShapes.size() == 0)
		return;

	for (CurveShape& Shape : EdgeShapes)
		Window.draw(Shape);
}

void CurveShape::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform(); // getTransform() is defined by sf::Transformable
	target.draw(Curve, states);
}

std::size_t CurveShape::getPointCount() const
{
	return listPoints.at(0).size(); // fixed, but could be an attribute of the class if needed
}

sf::Vector2f CurveShape::getPoint(std::size_t index) const
{

	//qDebug() << "index - " << index;
	sf::Vector2f Coord;
	Coord.x = listPoints.at(0).at(index).x();
	Coord.y = listPoints.at(0).at(index).y();
	return Coord;
}

QPointF CurveShape::GetCoord(QString &str, QRegExp &rx)  // UTILTITY METHOD IS USED TO PARSING SVG FILE WHILE GRID LINES IS BEING CREATING
{
	QPointF Point;
	rx.indexIn(str);
	Point.setX(rx.cap(1).toDouble()); 
	Point.setY(rx.cap(2).toDouble());
	return Point;
}

//METHODS TO GET COPY OF CURVES
//===========================================
void CurveShape::operator=(const CurveShape& Shape)
{
 Path.addPath(Shape.Path);
 listPoints.append(Shape.listPoints);
 SubPathEdge.append(Shape.SubPathEdge);
 Direction = Shape.Direction;

 for(int n = 0; n < Shape.Curve.getVertexCount(); n++)
 Curve.append(Shape.Curve[n]);

 PathMassive.append(Shape.PathMassive);
}

CurveShape& CurveShape::GetCopy()
{
	CurveShape* NewShape = new CurveShape;
	*NewShape = *this;
	return *NewShape;
}
//===========================================
