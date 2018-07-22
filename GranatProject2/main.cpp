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


	std::string GameDir = qgetenv("GAME_WORK_DIR");

	bool res = img.loadFromFile(GameDir + "/WORK_DIR/TERRAIN_ISOMETRIC_TILES/Tiles/with_grid/Grass2.png");
	res = border.loadFromFile(GameDir + "E:/WorkDir/2dworkdir/Terrain/1x1Border.png");


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

	float zoom = 1.0f;


	//ClassHero Hero;
	//CurveShape Shape;

	MapConteinerClass Map;
			view2.zoom(3);
	Map.CreateMapFromFile(QString("E:/WorkDir/WORK_DIR/MAPS_TILED/TestMapBig.tmx"));

	//SVGCurveContainer Contaner;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
			{
				zoom = 0.8;
			view2.zoom(zoom);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				zoom = 1.25;
			view2.zoom(zoom);
			}

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



		if (clock.getElapsedTime().asMilliseconds() >= 50)
		{
			clock.restart();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				view2.move(-64, 0);
				x_center -= 64;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				view2.move(64, 0);
				x_center += 64;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				view2.move(0, -64);
				y_center -= 64;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				view2.move(0, 64);
				y_center += 64;
			}


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


