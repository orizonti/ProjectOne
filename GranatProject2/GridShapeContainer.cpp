#include <GridShapeContainer.h>

void GridShapeContainer::AddCurves(QString path)
{
	    QString GameDir = qgetenv("GAME_WORK_DIR");
		QFile newXMLFile(GameDir + "/WORK_DIR/TERRAIN_ISOMETRIC_TILES/PNG HILL TILES/TilesHill/" + path);
		bool result = newXMLFile.open(QIODevice::ReadOnly);
		QDomDocument newDomDoc;
		result = newDomDoc.setContent(&newXMLFile);



		QDomNodeList GroupsNodes = newDomDoc.documentElement().firstChild().nextSibling().toElement().elementsByTagName("g");


        //DOWNLOAD SVG GRID LINES
		for (int n = 0; n <= 1; n++)
		{

			QDomNode currentNod = GroupsNodes.at(n).firstChild();

			while (!currentNod.isNull())
			{
				QDomElement newElement = currentNod.toElement();

				if (newElement.tagName() == "path")
				{
					CurveShape newShape;
					newShape.Direction = n;
					newShape.AddCurves(newElement);

					if(n == 0)
					CurvesHoriz.append(newShape);
					if(n == 1)
					CurvesVert.append(newShape);
				}

				currentNod = currentNod.nextSibling();
			}
		}
		
		qDebug() << "=====================================================";
		qDebug() << "CREATE SUBB CELL PATH_PAINTER TO DEFINE CELL PRESSED";
		

	for (int y = 0; y < CurvesHoriz.size()-1; y++)
	{
		for (int x = 0; x < CurvesVert.size()-1; x++)
		{

				QPainterPath PathBottom = CurvesHoriz[y].SubPathEdge[x+1].toReversed();
				QPainterPath PathUp =      CurvesHoriz[y+1].SubPathEdge[x+1];

				QPainterPath PathRibLeft = CurvesVert[x].SubPathEdge[y+1];
				QPainterPath PathRibRight = CurvesVert[x+1].SubPathEdge[y+1].toReversed();

				QVector<CurveShape> QuadShapes; 
				QuadShapes.append(CurveShape(PathBottom,0));
				QuadShapes.append(CurveShape(PathUp,0));
				QuadShapes.append(CurveShape(PathRibLeft,1));
				QuadShapes.append(CurveShape(PathRibRight,1));

				QuadeRangleShape QuadRangle(QuadShapes);
				this->SubCellShapes.append(QuadRangle);

					PathRibLeft.connectPath(PathUp);
					PathRibLeft.connectPath(PathRibRight);
					PathRibLeft.connectPath(PathBottom);
					PathRibLeft.closeSubpath();
					SubCellPathes.append(PathRibLeft);

		}

	}
		qDebug() << "=====================================================";
}

void CurveShape::DrawPainterPath(int offset_x = 0, int offset_y = 0)
{
	QPointF LastPoint;
	for (PathPoints sub_path : this->PathMassive)
	{
	QPainterPath sub_path_rib;
		if (sub_path.typePath == "MOVE")
		{
			Path.moveTo(sub_path.Points.at(0));

			LastPoint = sub_path.Points.at(0);
		}

		if (sub_path.typePath == "LINE")
		{
			Path.lineTo(sub_path.Points.at(0));

			sub_path_rib.moveTo(LastPoint);
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

	PainterPathToShape(Path);

}
void CurveShape::AddCurves(QPainterPath pathShape, int Dir)
{
	Direction = Dir;
	if (!pathShape.toSubpathPolygons().isEmpty())
	{

	PainterPathToShape(pathShape);
	qDebug() << "CREATE SHAPE ON PATH";
	}
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

	listPoints = path.toSubpathPolygons();

	int x_offset = 8;
	int y_offset = -4;

	if (this->Direction == 1)
		y_offset = 4;

	for (QPointF point : listPoints.at(0))
	{

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

	}

	DrawPainterPath();
	update();
}

void GridShapeContainer::SetOffset(int x, int y)
{
	for (CurveShape& Shape : this->CurvesVert)
		Shape.move(x, y);
	for (CurveShape& Shape : this->CurvesHoriz)
		Shape.move(x, y);

	for(CurveShape& Shape : this->ContourShapes)
		Shape.move(x, y);

}

void GridShapeContainer::SetPosition(int x, int y)
{
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
	for(CurveShape& Shape : this->ContourShapes)
		Window.draw(Shape);
}

void GridShapeContainer::DrawCell(sf::RenderWindow& Window, int NumberCell)
{
	this->SubCellShapes[NumberCell].SetColor(sf::Color::Red);
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


	ContourShapes.append(CurvesHoriz.first().GetCopy());
	ContourShapes.append(CurvesHoriz.last().GetCopy());
	ContourShapes.append( CurvesVert.first().GetCopy());
	ContourShapes.append( CurvesVert.last().GetCopy());

	for(auto Shape: ContourShapes)
	Shape.SetColor(sf::Color::Red);


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
