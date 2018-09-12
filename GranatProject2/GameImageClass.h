#pragma once
#include "HeaderAndStructures.h"
#include "AnimationSet.h"
#include "GameCoord.h"

class AnimationSetContainer;
class SimpleImage
{
public:
	SimpleImage();
	SimpleImage(sf::Texture& tex);
	SimpleImage(const SimpleImage& Image);
	SimpleImage(SimpleImage&& Image);
	~SimpleImage();

virtual	void DisplayImage(sf::RenderWindow& Window);

	void SetTexture(sf::Texture& Texture);
	void SetPositionImage(float x, float y);
	void TranslateImage(float x, float y);
	void SetPositionImage(QPair<float,float> Coord);

	void SetPositionOnMap(float iso_x, float iso_y);
	void SetPositionOnMap(QPair<float,float> IsoCoord);


	void SetObjectDirection(Direction Dir);
	void SetElevation(double Elevation);
	void Elevate(double Elevation);

	QPair<float,float>  OffsetToImage;
protected:
	QString Type = "None";
	GameCoord ImagePosOnMap;
	Direction CurrentDir = Right;
	std::shared_ptr<sf::Sprite>  Sprite;
	double DecElevation = 0;
	
};

class AnimationImage : public SimpleImage
{
public:
	AnimationImage();
	~AnimationImage();
	AnimationImage(QString UnitType);
	AnimationImage(const AnimationImage& Image);
	AnimationImage(AnimationImage&& Image);

	static std::shared_ptr<AnimationSetContainer> Animations;
	static void LoadAnimationSet(QString PathAnimation);

	void DisplayImage(sf::RenderWindow& Window);

private:
	std::shared_ptr<AnimationSet> AnimationImages = NULL;
	int         CurrentFrame = 0;

	void IterateAnimation();
};



