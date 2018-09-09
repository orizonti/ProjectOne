#include "GameImageClass.h"
#include "AnimationSetContainer.h"

std::shared_ptr<AnimationSetContainer> AnimationImage::Animations = NULL;

AnimationImage::AnimationImage()
{
	//qDebug() << "ANIMATION IMAGE DEFAULT CONSTRUCTOR";
}
AnimationImage::AnimationImage(QString UnitType)
{
	//qDebug() << "CREATE ANIMATION IMAGE TO TYPE - " << UnitType;
							this->CurrentDir = Direction::Right;
	AnimationImages = Animations->GetUnitAnimationSet(UnitType);
	this->Sprite->setTexture(AnimationImages->GetTexture(CurrentDir,0));
	Type = UnitType;
}

AnimationImage::AnimationImage(AnimationImage&& Image)
{
	//qDebug() << "ANIMATION IMAGE CONSTRUCTOR OF MOVING";
	this->Sprite = Image.Sprite;
				   Image.Sprite = NULL;
	this->AnimationImages = Image.AnimationImages;
							Image.AnimationImages = NULL;

							this->CurrentDir = Image.CurrentDir;

}

AnimationImage::AnimationImage(const AnimationImage& Image)
{
	//qDebug() << "         CREATE ANIMATION IMAGE COPY of TYPE - " << Image.Type << "PTR - " << this;
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

void AnimationImage::DisplayImage(sf::RenderWindow& Window)
{
	Window.draw(*Sprite);
	this->IterateAnimation();
}

void GroupImage::DisplayImage(sf::RenderWindow& Window)
{
	        Images[3]->DisplayImage(Window);
			Images[0]->DisplayImage(Window);
			Images[1]->DisplayImage(Window);
			Images[2]->DisplayImage(Window);
	//for (auto Image : Images)
	//	Image->DisplayImage(Window);
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
	//qDebug() << "SIMPLE IMAGE COPY CONSTRUCTOR - " << this;
	Sprite = std::shared_ptr<sf::Sprite>(new sf::Sprite(*Image.Sprite->getTexture()));
}
SimpleImage::SimpleImage(SimpleImage&& Image)
{
	Sprite = Image.Sprite;
			 Image.Sprite = NULL;
}
AnimationImage::~AnimationImage()
{
	//qDebug() << "ANIMATION IMAGE DESTRUCTOR";
}

SimpleImage::~SimpleImage()
{
	//qDebug() << "SIMPLE IMAGE DESTRUCTOR";
}

void AnimationImage::LinkAnimationSet(std::shared_ptr<AnimationSet> Animation)
{
	AnimationImages = Animation;
}

GroupImage::GroupImage()
{
	//qDebug() << "GROUP IMAGE DEFAULT CONSTRUCTOR";
	
}
GroupImage::GroupImage(const GroupImage& Image) : Images(Image.Images), 
                                                  OffsetToImage(Image.OffsetToImage)
{
	//qDebug() << "GROUP COPY CONSTRUCTOR";
	GroupSize = Image.GroupSize;
}

GroupImage::GroupImage(const GroupImage&& Image) : Images(std::move(Image.Images)),
                                                   OffsetToImage(std::move(OffsetToImage))
{

	//qDebug() << "GROUP MOVE CONSTRUCTOR";
	GroupSize = Image.GroupSize;
}

void GroupImage::operator=(const GroupImage& Image)
{
	//qDebug() << "GROUP IMAGE COPY = OPERATOR";
	this->Images.append(Image.Images);
	this->OffsetToImage.append(Image.OffsetToImage);
	this->GroupSize = Image.GroupSize;
}

void GroupImage::operator=(const GroupImage&& Image)
{
	//qDebug() << "GROUP IMAGE MOVE = OPERATOR";
	Images.append(std::move(Image.Images));
	this->OffsetToImage.append(std::move(Image.OffsetToImage));
	this->GroupSize = Image.GroupSize;
}

GroupImage::GroupImage(const SimpleImage& Image, int Size)
{
	this->GroupSize = Size;

	if (Size <= 4)
	{
		OffsetToImage.append(QPair<float, float>(0.5, 0));
		OffsetToImage.append(QPair<float, float>(0.5, 0.5));
		OffsetToImage.append(QPair<float, float>(-0.5, 0));
		OffsetToImage.append(QPair<float, float>(0, 0.5));
	}

	for (int n = 0; n < Size; n++)
	{
		Images.append(std::shared_ptr<SimpleImage>(new SimpleImage(Image)));
	}
}

GroupImage::GroupImage(const AnimationImage& Image, int Size)
{
	this->GroupSize = Size;
	//qDebug() << "GROUP IMAGE CONSTRUCTOR SIZE - " << Size;

	if (Size <= 4)
	{
//		OffsetToImage.append(QPair<float, float>(0.4, -0.1));
//		OffsetToImage.append(QPair<float, float>(1, -0.1));
//		OffsetToImage.append(QPair<float, float>(0.4, -0.5));
//		OffsetToImage.append(QPair<float, float>(1, -0.5));

		OffsetToImage.append(QPair<float, float>(0.5, 0));
		OffsetToImage.append(QPair<float, float>(1, 0));
		OffsetToImage.append(QPair<float, float>(0.5, -0.5));
		OffsetToImage.append(QPair<float, float>(1, -0.5));

	//	OffsetToImage.append(QPair<float, float>(0.4, 0.1));    //BR
	//	OffsetToImage.append(QPair<float, float>(1.1, 0.1));      //TR
	//	OffsetToImage.append(QPair<float, float>(0.4, -0.7)); //BL
	//	OffsetToImage.append(QPair<float, float>(1.1, -0.7)); //TL
	}

	for (int n = 0; n < Size; n++)
	{
		//qDebug() << "--------------------------------------------";
		//qDebug() << "APPEND NEW ANIMATION IMAGE - " << n;
		Images.append(std::shared_ptr<AnimationImage>(new AnimationImage(Image)));
	}
}

void GroupImage::AppendImage(std::shared_ptr<SimpleImage> Image)
{
	GroupSize++;
	Images.append(Image);
}

void GroupImage::AppendImage(AnimationImage&& Image)
{
		GroupSize++;
		Images.append(std::shared_ptr<AnimationImage>(new AnimationImage(std::forward<AnimationImage>(Image))));
}
void GroupImage::AppendImage(SimpleImage&& Image)
{
		Images.append(std::shared_ptr<SimpleImage>(new SimpleImage(std::forward<SimpleImage>(Image))));
}


void GroupImage::TranslateElevation(QVector<double>& Translation)
{
	qDebug() << "ELEVATE GROUP - " << Translation;
	int size = Translation.size();

			Images[0]->Elevate(Translation[2]);
			Images[1]->Elevate(Translation[3]);

			Images[2]->Elevate(Translation[0]);
	        Images[3]->Elevate(Translation[1]);

	//if(Images.size() < Translation.size())
	//    size = Images.size();

	//for (int n = 0; n < size; n++)
	//		Images[n]->Elevate(Translation[n]);
}

void GroupImage::SetPositionOnMap(float iso_x, float iso_y)
{
	//qDebug() << "SET GROUP IMAGE TO MAP POS - " << iso_x << iso_y;
	for (int n = 0; n < GroupSize; n++)
	{
		Images[n]->SetPositionOnMap(iso_x + OffsetToImage[n].first,iso_y + OffsetToImage[n].second);
	}
}
void GroupImage::SetPositionOnMap(QPair<float,float> IsoCoord)
{
	//qDebug() << "SET GROUP IMAGE TO MAP POS - " << IsoCoord.first << IsoCoord.second;
	for (int n = 0; n < GroupSize; n++)
	{
		Images[n]->SetPositionOnMap(IsoCoord.first + OffsetToImage[n].first, IsoCoord.second + OffsetToImage[n].second);
	}
}


void GroupImage::SetDiretionMoving(Direction Dir)
{
	for (auto Image : Images)
	{
		Image->SetObjectDirection(Dir);
	}
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
	//qDebug() << "SET POS ON MAP IN IMAGE - " << this <<"COORD - " <<iso_x << iso_y;
	ImagePosOnMap.SetCoordIsometric(iso_x, iso_y);
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
	//qDebug() << "SET DECART COORD - " << Coord.first << Coord.second;
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
	qDebug() << "CURRENT ELEVATION - " << DecElevation;
}



