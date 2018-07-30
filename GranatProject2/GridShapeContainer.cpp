#include <GridShapeContainer.h>

void GridShapeContainer::AddCurves(QString file)
{
	    QString GameDir = qgetenv("GAME_WORK_DIR");
		QFile newXMLFile(GameDir + "/WORK_DIR/TERRAIN_ISOMETRIC_TILES/PNG HILL TILES/TilesHill/" + file);
		qDebug() << "SVG FILE LOAD - " << GameDir + "/WORK_DIR/TERRAIN_ISOMETRIC_TILES/PSD HILL TILES/TilesHill/L.svg";
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
					Curves.append(newShape);
				}

				currentNod = currentNod.nextSibling();
			}
		}

		
	//	for (CurveShape Shape : Curves)
	//	{
	//		int CountPoints;
	//		CountPoints = Shape.listPoints.at(0).size();

	//		for (int n = 0; n < CountPoints; n++)
	//			this->Points.append(Shape.getPoint(n));

	//	}

}

void CurveShape::DrawPainterPath(int offset_x = 0, int offset_y = 0)
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
	for (CurveShape& Shape : this->Curves)
		Shape.move(x, y);
}

void GridShapeContainer::SetPosition(int x, int y)
{
	for (CurveShape& Shape : this->Curves)
		Shape.setPosition(x, y);
}

void GridShapeContainer::DrawGrid(sf::RenderWindow& Window)
{
	for (CurveShape Shape : this->Curves)
		Window.draw(Shape);
}

