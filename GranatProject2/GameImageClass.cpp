#include "GameImageClass.h"



StaticImage::StaticImage()
{


}


StaticImage::~StaticImage()
{
}

void AnimationImage::LinkAnimationSet(std::shared_ptr<AnimationSet> Animation)
{
	AnimationImages = Animation;
}

GroupImage::GroupImage()
{

}

GroupImage::GroupImage(int Size, std::shared_ptr<AnimationImage> Animation)
{
	this->GroupSize = Size;
	for (int n = 0; n < Size; n++)
	{

		Images.append(Animation);
		ImagesPos.append(GameCoord());
	}
}

void GroupImage::IterateAnimation(Direction Dir)
{
	for (auto Image : Images)
	{
		Image->IterateAnimation(Dir);
	}
}

void GroupImage::SetTexture(sf::Texture& Texture)
{
	for (auto Image : Images)
	{
		Image->SetTexture(Texture);
	}
}

void GroupImage::SetImage(int Size, std::shared_ptr<AnimationImage> Image)
{

	this->GroupSize = Size;
	for (int n = 0; n < Size; n++)
	{

		Images.append(Image);
		ImagesPos.append(GameCoord());
	}
}

void GroupImage::SetPositionImage(int x, int y)
{
	ImagesPos[0].SetCoordIsometric(x, y);
	ImagesPos[1].SetCoordIsometric(x+0.5, y);
	ImagesPos[2].SetCoordIsometric(x, y+0.5);
	ImagesPos[3].SetCoordIsometric(x+0.5, y+0.5);
	
	for (int n = 0; n < GroupSize; n++)
	{
		Images[n]->SetPositionImage(ImagesPos[n].DecPos(0),ImagesPos[n].DecPos(1));
	}

}

void GroupImage::SetPositionImage(QPair<int, int> Coord)
{
	ImagesPos[0].SetCoordIsometric(Coord.first, Coord.second);
	ImagesPos[1].SetCoordIsometric(Coord.first+0.5, Coord.second);
	ImagesPos[2].SetCoordIsometric(Coord.first, Coord.second+0.5);
	ImagesPos[3].SetCoordIsometric(Coord.first+0.5, Coord.second+0.5);
	
	for (int n = 0; n < GroupSize; n++)
	{
		Images[n]->SetPositionImage(ImagesPos[n].DecPos(0),ImagesPos[n].DecPos(1));
	}
}

void GroupImage::SetDiretionMoving(Direction Dir)
{
	for (auto Image : Images)
	{
		Image->SetDiretionMoving(Dir);
	}
}

void AnimationImage::IterateAnimation(Direction Dir)
{
	if (this->AnimationImages == NULL)
		return;

	if (Dir != CurrentDir)
	{
		CurrentFrame = 0;
		Dir = CurrentDir;
	}

		if (CurrentFrame == 18)
			CurrentFrame = 0;

		this->Sprite.setTexture(AnimationImages->GetTexture(Dir,CurrentFrame));

		CurrentFrame++;

}


void StaticImage::SetPositionImage(QPair<int, int> Coord)
{
		Sprite.setPosition(Coord.first,Coord.second);
}
void StaticImage::SetPositionImage(int x, int y)
{
		Sprite.setPosition(x,y);
}


void StaticImage::SetDiretionMoving(Direction Dir)
{
	this->CurrentDir = Dir;
}



void StaticImage::SetTexture(sf::Texture& Texture)
{
//	this->CurrentSprite.setTexture(Texture);
//	this->CurrentSprite1.setTexture(Texture);
//	this->CurrentSprite2.setTexture(Texture);
//	this->CurrentSprite3.setTexture(Texture);
}
