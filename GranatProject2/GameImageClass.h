#pragma once
#include "HeaderAndStructures.h"
#include "AnimationSet.h"
#include "GameCoord.h"


class StaticImage
{
public:
	StaticImage();
	~StaticImage();


	void SetTexture(sf::Texture& Texture);
	void SetPositionImage(int x, int y);
	void SetPositionImage(QPair<int,int> Coord);

	void SetDiretionMoving(Direction Dir);

	sf::Sprite  Sprite;

	Direction CurrentDir = Right;

};

class AnimationImage : public StaticImage
{
public:
	void IterateAnimation(Direction Dir);
	void LinkAnimationSet(std::shared_ptr<AnimationSet> Animation);
	std::shared_ptr<AnimationSet> AnimationImages = NULL;
	int         CurrentFrame = 0;
};


class GroupImage : public AnimationImage
{
public:
	GroupImage();
	GroupImage(int Size, std::shared_ptr<AnimationImage> Animation = 0);

	~GroupImage()
	{
	}

	void IterateAnimation(Direction Dir);

	void SetTexture(sf::Texture& Texture);
	void SetImage(int Size, std::shared_ptr<AnimationImage> Image);


	void SetPositionImage(int x, int y);
	void SetPositionImage(QPair<int,int> Coord);
	void SetDiretionMoving(Direction Dir);
private:
	QVector<GameCoord>      ImagesPos;
	QVector<std::shared_ptr<AnimationImage>> Images;
	int GroupSize;
};

