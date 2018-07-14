#pragma once

#include <qstring>
#include "SFML\Graphics.hpp"
#include <Eigen/Dense>
#include <QDebug>
#include <QDir>

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



class UnitObjectClass
{
public:
	UnitObjectClass();
	~UnitObjectClass();
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

	void SetCoordDecart(int x, int y);
	void SetCoordIsometric(int x, int y);
	void SetCoodScreen(double x, double y, int x_center, int y_center);


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
	ClassHero()  // Test comment
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

void CoordClass::SetCoordDecart(int x, int y)
{
	DecPos(0) = x;
	DecPos(1) = y;
	IsoPos = n*DecPos;
}

void CoordClass::SetCoordIsometric(int x, int y)
{
	qDebug() << "POS HERO SET COORD - " << x << y;
	IsoPos(0) = x - 1;
	IsoPos(1) = y - 1;

	DecPos = m*IsoPos * 64;
	qDebug() << "IsoPos - " << x << y << "DecPos - " << DecPos(0) << DecPos(1);
	ScreenPos.x = DecPos(0);
	ScreenPos.y = 270 - DecPos(1);

}


inline void CoordClass::SetCoodScreen(double x, double y, int x_center, int y_center)
{
	DecPos(0) = x / 64.0 + x_center / 64;
	DecPos(1) = -(y - 350.0) / 64.0 + y_center / 64;

	IsoPos = n*DecPos;

	IsoPos(0) = floor(IsoPos(0));
	IsoPos(1) = floor(IsoPos(1));
}
