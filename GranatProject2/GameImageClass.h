#pragma once
#include "HeaderAndStructures.h"

class ClassAnimationSet
{
public:
	ClassAnimationSet(int CountFrame, int CountSet)
	{
		FrameCount = CountFrame;
		SetCount = CountSet;
	}
	int FrameCount = 20;
	int SetCount   = 8;

	void UploadAnimation(QString Name, QString SetDir);

	
	sf::Texture& GetTexture(Direction Dir, int Frame);
	QVector<QVector< std::shared_ptr<sf::Texture> >> TexturesSet;


};

class GameImage
{
public:
public:
	GameImage();
	~GameImage();


	void SetTexture(sf::Texture& Texture);
	void SetPositionImage(int x, int y);
	void SetPositionImage(QPair<int,int> Coord);
	void LinkAnimationSet(std::shared_ptr<ClassAnimationSet> Animation);

	void SetDiretionMoving(Direction Dir);

	void IterateAnimation(Direction Dir);

	std::shared_ptr<ClassAnimationSet> AnimationSet = NULL;

	sf::Sprite  CurrentSprite;
	sf::Sprite  CurrentSprite1;
	sf::Sprite  CurrentSprite2;
	sf::Sprite  CurrentSprite3;

	int         CurrentFrame = 0;
	Direction CurrentDir = Right;

};

