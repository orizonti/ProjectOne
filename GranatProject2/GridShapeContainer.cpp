#include <GridShapeContainer.h>

void GridShapeContainer::AddCurves(QString file)
{
	    QString GameDir = qgetenv("GAME_WORK_DIR");
		QFile newXMLFile(GameDir + "/WORK_DIR/TERRAIN_ISOMETRIC_TILES/PNG HILL TILES/TilesHill/" + file);
		bool result = newXMLFile.open(QIODevice::ReadOnly);
		qDebug() << "FILE OPEN - " << newXMLFile.isOpen();
		QDomDocument newDomDoc;
		result = newDomDoc.setContent(&newXMLFile);



		QDomNodeList GroupsNodes = newDomDoc.documentElement().firstChild().nextSibling().toElement().elementsByTagName("g");

		qDebug() << "FIRST CHILD NODE -" << GroupsNodes.size();


		for (int n = 0; n <= 1; n++)
		{

			QDomNode currentNod = GroupsNodes.at(n).firstChild();

			while (!currentNod.isNull())
			{
				qDebug() << "New shape direction - " << n;
				QDomElement newElement = currentNod.toElement();

				qDebug() << "*************************************";
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
		
		CreateLinePathes();
}

void CurveShape::DrawPainterPath(int offset_x = 0, int offset_y = 0)
{

	QPainterPath sub_path_rib;
	for (PathPoints sub_path : this->PathMassive)
	{
		if (sub_path.typePath == "MOVE")
		{
			Path.moveTo(sub_path.Points.at(0));
			sub_path_rib.moveTo(sub_path.Points.at(0));
		}

		if (sub_path.typePath == "LINE")
		{
			Path.lineTo(sub_path.Points.at(0));
			sub_path_rib.lineTo(sub_path.Points.at(0));
		}

		if (sub_path.typePath == "CUBIC")
		{
			Path.cubicTo(sub_path.Points.at(0), sub_path.Points.at(1), sub_path.Points.at(2));
			sub_path_rib.cubicTo(sub_path.Points.at(0), sub_path.Points.at(1), sub_path.Points.at(2));
     	}
	SubPathEdge.append(sub_path_rib);
	}
	qDebug() << "SUB PATH EDGE SIZE  - " << SubPathEdge.size();

	PainterPathToShape(Path);

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

}

void GridShapeContainer::DrawContour(sf::RenderWindow& Window)
{
	for(CurveShape& Shape : this->ContourShapes)
		Window.draw(Shape);
}

void GridShapeContainer::DrawGrid(sf::RenderWindow& Window)
{

//	if (CurvesVert.isEmpty())
//		return;
	for (CurveShape Shape : this->CurvesVert)
		Window.draw(Shape);

	for (CurveShape Shape : this->CurvesHoriz)
		Window.draw(Shape);


//	Window.draw(CurvesHoriz.first());
//	Window.draw(CurvesHoriz.last());

//	Window.draw(CurvesVert.first());
//	Window.draw(CurvesVert.last());

//	if(this->draw_contour_flag)
//	this->DrawContour(Window);
}


void GridShapeContainer::CreateLinePathes()
{
	if (CurvesVert.isEmpty())
		return;

	QVector<QPainterPath> EdgeVertStart = CurvesVert.first().SubPathEdge;
	QVector<QPainterPath> EdgeVertEnd = CurvesVert.last().SubPathEdge;

	QVector<QPainterPath> EdgeHorizStart = CurvesHoriz.first().SubPathEdge;
	QVector<QPainterPath> EdgeHorizEnd   = CurvesHoriz.last().SubPathEdge;
	
	QPainterPath PathUp;
	QPainterPath PathBottom;
	QPainterPath PathRibLeft;
	QPainterPath PathRibRight;

		//=======================================================
	qDebug() << "CREATE PATH CONTOUR";
	PathUp = CurvesHoriz.first().Path.toReversed();
	PathBottom = CurvesHoriz.last().Path;
	PathRibLeft = CurvesVert.first().Path;
	PathRibRight = CurvesVert.last().Path.toReversed();


	ContourShapes.append(CurvesHoriz.first().GetCopy());
	ContourShapes.append(CurvesHoriz.last().GetCopy());
	ContourShapes.append( CurvesVert.first().GetCopy());
	ContourShapes.append( CurvesVert.last().GetCopy());

//	for(auto Shape: ContourShapes)
//	Shape.SetColor(sf::Color::Red);


		PathRibLeft.connectPath(PathBottom);
		PathRibLeft.connectPath(PathRibLeft);
		PathRibLeft.connectPath(PathUp);
		PathRibLeft.closeSubpath();
		PathContour = PathRibLeft;
	
		//=======================================================

		qDebug() << "CREATE PATH LINE VERT";
		QVector<CurveShape> StripeShape;
	for (int n = 0; n < CurvesVert.size()-1; n++)
	{
		PathUp = EdgeHorizStart.at(n).toReversed();
		PathBottom = EdgeHorizEnd.at(n);

		PathRibLeft = CurvesVert.at(n).Path;
		PathRibRight = CurvesVert.at(n+1).Path.toReversed();

//		CurveShape newShape =  CurvesHoriz.first();
//		CurveShape newShape2 = CurvesHoriz.last();
//		CurveShape newShape3 = CurvesVert.first();
//		CurveShape newShape4=  CurvesVert.last();


//	ContourShapes.append(newShape);
//	ContourShapes.append(newShape2);
//	ContourShapes.append(newShape3);
//	ContourShapes.append(newShape4);

		PathRibLeft.connectPath(PathBottom);
		PathRibLeft.connectPath(PathRibRight);
		PathRibLeft.connectPath(PathUp);
		PathRibLeft.closeSubpath();
		PathLineVert.append(PathRibLeft);
	}
	qDebug() << "PATH LINE VERT COUNT - " << PathLineVert.size();


	qDebug() << "CREATE PATH LINE HORIZ";
	for (int n = 0; n < CurvesHoriz.size()-1; n++)
	{
		PathUp = CurvesHoriz.at(n).Path.toReversed();
		PathBottom = CurvesHoriz.at(n+1).Path;

		PathRibLeft = EdgeVertStart.at(n);
		PathRibRight = EdgeVertEnd.at(n).toReversed();

		PathRibLeft.connectPath(PathBottom);
		PathRibLeft.connectPath(PathRibLeft);
		PathRibLeft.connectPath(PathUp);
		PathRibLeft.closeSubpath();
		PathLineHoriz.append(PathRibLeft);
	}
	qDebug() << "PATH LINE HORIZ COUNT -" << PathLineHoriz.size();
}

