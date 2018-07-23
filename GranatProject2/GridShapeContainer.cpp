#include <GridShapeContainer.h>

void GridShapeContainer::AddCurves(QString path)
{
	    QString GameDir = qgetenv("GAME_WORK_DIR");
		qDebug() << "SVG FILE LOAD - " << GameDir + "/444.svg";
		QFile newXMLFile(GameDir + "/444.svg");
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