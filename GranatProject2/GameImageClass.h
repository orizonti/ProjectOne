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


	void SetPositionImage(int x, int y);

	void SetDiretionMoving(Direction Dir);

	void IterateAnimation(Direction Dir);

	ClassAnimationSet* AnimationSet;

	sf::Sprite  CurrentSprite;
	int         CurrentFrame = 0;
	Direction CurrentDir = Right;

};

