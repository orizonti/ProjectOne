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

class CurveShape : public sf::Shape
{
public:

	explicit CurveShape()
	{
		Curve.setPrimitiveType(sf::LineStrip);
		DrawPainterPath();
		update();
	}

	QPainterPath Path;
	QList<QPolygonF> listPoints;

	sf::VertexArray Curve;


	virtual std::size_t getPointCount() const
	{
		return listPoints.at(0).size(); // fixed, but could be an attribute of the class if needed
	}

	virtual sf::Vector2f getPoint(std::size_t index) const
	{

		qDebug() << "index - " << index;
		sf::Vector2f Coord;
		Coord.x = listPoints.at(0).at(index).x();
		Coord.y = listPoints.at(0).at(index).y();
		return Coord;
	}

private:
	void DrawPainterPath();

};




class GameImage
{
public:
	GameImage(std::string path_to_Image = "")
	{
		hero.loadFromFile("E:/WorkDir/WORK_DIR/DEMO_UNIT_ANIMATION/running/RUNNING E0.png");
		textureHero.loadFromImage(hero);
		spriteHero.setTexture(textureHero);

		for (int n = 0; n <= 11; n++)
		{
			QString file = QString("E:/WorkDir/WORK_DIR/DEMO_UNIT_ANIMATION/running/RUNNING E%1.png").arg(n);
			textureHeroMove[n].loadFromFile(file.toStdString());
		};

	}
	int CurrentFrame = 0;

	sf::Image hero;
	sf::Texture textureHero;
	sf::Sprite spriteHero;

	sf::Texture textureHeroMove[12];

	void SetPositionImage(int x, int y)
	{
		spriteHero.setPosition(x, y);
	}

	void SetDiretionMoving(int direction)
	{

	}

	void IterateAnimation()
	{
		CurrentFrame++;
		if (CurrentFrame == 11)
			CurrentFrame = 0;

		spriteHero.setTexture(this->textureHeroMove[CurrentFrame]);
	}

};

class DisplayMapClass
{
public:
	DisplayMapClass()
	{

	}
	GameImage MapTile;

};

class CoordClass
{
public:
	CoordClass()
	{
		m << 1, 1,
			-0.5, 0.5;

		n << 0.5, -1,
			0.5, 1;

		IsoPos(0) = 0;
		IsoPos(1) = 0;
	};

	Eigen::Matrix2d n;
	Eigen::Matrix2d m;


	Eigen::Vector2d DecPos;
	Eigen::Vector2d IsoPos;
	sf::Vector2f ScreenPos;

	void SetCoordDecart(int x, int y)
	{
		DecPos(0) = x;
		DecPos(1) = y;
		IsoPos = n*DecPos;
	}

	void SetCoordIsometric(int x, int y)
	{
		qDebug() << "POS HERO SET COORD - " << x << y;
		IsoPos(0) = x - 1;
		IsoPos(1) = y - 1;

		DecPos = m*IsoPos * 64;
		qDebug() << "IsoPos - " << x << y << "DecPos - " << DecPos(0) << DecPos(1);
		ScreenPos.x = DecPos(0);
		ScreenPos.y = 270 - DecPos(1);

	}

	void SetCoodScreen(double x, double y, int x_center, int y_center)
	{

		DecPos(0) = x / 64.0 + x_center / 64;
		DecPos(1) = -(y - 350.0) / 64.0 + y_center / 64;

		IsoPos = n*DecPos;

		IsoPos(0) = floor(IsoPos(0));
		IsoPos(1) = floor(IsoPos(1));
	}

	bool operator==(CoordClass &Coord)
	{
		if (this->DecPos == Coord.DecPos)
			return true;
		else
			return false;

	}
};


class ClassHero
{
public:
	ClassHero()
	{
		SetPosition(3, 4);
		SetDestination(3, 4);
	}

	int CurrentFrame = 0;

	CoordClass PosHero;
	CoordClass PosDestinationHero;
	GameImage ImageHero;


	void MoveHero();
	void SetPosition(int x, int y);
	void SetDestination(int x, int y);

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

	bool res = img.loadFromFile("E:/WorkDir/2dworkdir/Terrain/1x1Green.png");
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


	ClassHero Hero;
	//CurveShape Shape;


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
				qDebug() << "IsoVect - " << IsoVect2(0) << IsoVect2(1) << "DecVect - " << DecVect2(0) << DecVect2(1);


			}

			if (event.type == sf::Event::MouseButtonPressed)
			{

				DecVect2(0) = double(event.mouseMove.x) / 64.0 + x_center / 64;
				DecVect2(1) = -(double(event.mouseMove.y) - 350.0) / 64.0 + y_center / 64;

				IsoVect2 = n*DecVect2;

				IsoVect2(0) = floor(IsoVect2(0));
				IsoVect2(1) = floor(IsoVect2(1));


				Hero.SetDestination(IsoVect2(0), IsoVect2(1));

			}
		}



		if (clock.getElapsedTime().asMilliseconds() >= 300)
		{
			clock.restart();

			window.clear();

			for (int x = 0; x <= 40; x++)
			{
				for (int y = 0; y <= 40; y++)
				{
					IsoVect(0) = x;
					IsoVect(1) = y;

					DecVect = m*IsoVect * 64;

					sprite.setPosition(0 + DecVect(0), 320 + DecVect(1));
					window.draw(sprite);
				}
			}

			//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			//	{
			//		view2.move(-64, 0);
			//		x_center -= 64;
			//	}

			//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			//	{
			//		view2.move(64, 0);
			//		x_center += 64;
			//	}
			//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			//	{
			//		view2.move(0, -64);
			//		y_center -= 64;
			//	}
			//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			//	{
			//		view2.move(0, 64);
			//		y_center += 64;
			//	}

			Hero.MoveHero();

			spriteBorder.setPosition(DecVect2(0), DecVect2(1));
			window.draw(spriteBorder);
			window.draw(Hero.ImageHero.spriteHero);
			//window.draw(Shape.Curve);
			window.setView(view2);
			window.display();
		}



	}

	return 0;
	//return a.exec();
}

void ClassHero::MoveHero()
{

	if (this->PosHero == PosDestinationHero)
		return;



	double x1 = this->PosHero.DecPos(0);
	double y1 = this->PosHero.DecPos(1);


	double x2 = PosDestinationHero.DecPos(0);
	double y2 = PosDestinationHero.DecPos(1);

	this->PosHero.DecPos(0) = PosHero.DecPos(0) + 4;
	this->PosHero.DecPos(1) = (-(x1*y2 - x2*y1) - (y1 - y2)*(PosHero.DecPos(0))) / (x2 - x1);

	ImageHero.SetPositionImage(PosHero.DecPos(0), PosHero.DecPos(1));
	ImageHero.IterateAnimation();
}

void ClassHero::SetDestination(int x, int y)
{
	PosDestinationHero.SetCoordIsometric(x, y);
}


void ClassHero::SetPosition(int x, int y)
{
	this->PosHero.SetCoordIsometric(x, y);
	qDebug() << "Set position - " << x << y;
	this->ImageHero.spriteHero.setPosition(PosHero.ScreenPos.x, PosHero.ScreenPos.y);
}

void CurveShape::DrawPainterPath()
{
	Path.moveTo(56, 268);
	Path.quadTo(QPointF(73, 112), QPointF(188, 170));
	Path.cubicTo(QPointF(303, 230), QPointF(380, 228), QPointF(320, 166));


	listPoints = Path.toSubpathPolygons();

	for (QPointF point : listPoints.at(0))
	{

		sf::Vertex vertex;
		vertex.position = sf::Vector2f(point.x(), point.y());
		vertex.color = sf::Color::Green;
		Curve.append(vertex);

		qDebug() << point.toPoint().x() << point.toPoint().y();
	}

	qDebug() << "Nuber polygons - " << listPoints.at(0).size();


	listPoints = Path.toSubpathPolygons();

	for (QPointF point : listPoints.at(0))
	{

		sf::Vertex vertex;
		vertex.position = sf::Vector2f(point.x(), point.y());
		vertex.color = sf::Color::Green;
		Curve.append(vertex);

		qDebug() << point.toPoint().x() << point.toPoint().y();
	}

	qDebug() << "Nuber polygons - " << listPoints.at(0).size();
}