#include "GameImageClass.h"
#include "AnimationSetContainer.h"

std::shared_ptr<AnimationSetContainer> AnimationImage::Animations = NULL;

AnimationImage::AnimationImage()
{
}
AnimationImage::AnimationImage(QString UnitType)
{
							this->CurrentDir = Direction::Right;
	AnimationImages = Animations->GetUnitAnimationSet(UnitType);
	this->Sprite->setTexture(AnimationImages->GetTexture(CurrentDir,0));
	Type = UnitType;
}

AnimationImage::AnimationImage(AnimationImage&& Image)
{
	this->Sprite = Image.Sprite;
				   Image.Sprite = NULL;
	this->AnimationImages = Image.AnimationImages;
							Image.AnimationImages = NULL;

							this->CurrentDir = Image.CurrentDir;

}

AnimationImage::AnimationImage(const AnimationImage& Image)
{
	this->Sprite = std::shared_ptr<sf::Sprite>(new sf::Sprite(*Image.Sprite->getTexture()));
	this->AnimationImages = Image.AnimationImages;
	this->CurrentDir = Image.CurrentDir;
	this->CurrentFrame = Image.CurrentFrame;
	this->Type = Image.Type;

}

void SimpleImage::DisplayImage(sf::RenderWindow& Window)
{
	Window.draw(*Sprite);
}

void AnimationImage::LoadAnimationSet(QString PathAnimation)
{
   Animations = std::shared_ptr<AnimationSetContainer>(new AnimationSetContainer());
   Animations->UploadAnimationSets(PathAnimation);
}

void AnimationImage::DisplayImage(sf::RenderWindow& Window)
{
	Window.draw(*Sprite);
	this->IterateAnimation();
}



SimpleImage::SimpleImage()
{
	this->Sprite = std::make_shared<sf::Sprite>(sf::Sprite());
}

SimpleImage::SimpleImage(sf::Texture& tex)
{
	this->Sprite = std::make_shared<sf::Sprite>(sf::Sprite(tex));
}

SimpleImage::SimpleImage(const SimpleImage& Image)
{
	Sprite = std::shared_ptr<sf::Sprite>(new sf::Sprite(*Image.Sprite->getTexture()));
}
SimpleImage::SimpleImage(SimpleImage&& Image)
{
	Sprite = Image.Sprite;
			 Image.Sprite = NULL;
}
AnimationImage::~AnimationImage()
{
}

SimpleImage::~SimpleImage()
{
}


void AnimationImage::IterateAnimation()
{
	if (this->AnimationImages == NULL)
		return;


		if (CurrentFrame == 18)
			CurrentFrame = 0;

		this->Sprite->setTexture(AnimationImages->GetTexture(CurrentDir,CurrentFrame));

		CurrentFrame++;

}

void SimpleImage::SetPositionOnMap(float iso_x, float iso_y)
{
	ImagePosOnMap.SetCoordIsometric(iso_x + OffsetToImage.first, iso_y + OffsetToImage.second);
	ImagePosOnMap.translateDecart(0, -DecElevation);
	SetPositionImage(ImagePosOnMap.GetDecCoord());

}
void SimpleImage::SetPositionOnMap(QPair<float,float> IsoCoord)
{
	ImagePosOnMap.SetCoordIsometric(IsoCoord.first, IsoCoord.second);
	SetPositionImage(ImagePosOnMap.GetDecCoord());
}

void SimpleImage::SetPositionImage(QPair<float, float> Coord)
{
		Sprite->setPosition(Coord.first,Coord.second);
}
void SimpleImage::SetPositionImage(float x, float y)
{
		Sprite->setPosition(x,y);
}

void SimpleImage::TranslateImage(float x, float y)
{
	ImagePosOnMap.translate(x, y);
	SetPositionImage(ImagePosOnMap.GetDecCoord());
}

void SimpleImage::SetObjectDirection(Direction Dir)
{
	this->CurrentDir = Dir;
}

void SimpleImage::SetElevation(double Elevation)
{
	DecElevation = Elevation;
}

void SimpleImage::Elevate(double Elevation)
{
	DecElevation += Elevation;
}



