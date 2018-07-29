#include "GameImageClass.h"



GameImage::GameImage()
{


}

void GameImage::SetImage(QString TypeImage)
{
		for (int n = 0; n <= 11; n++)
		{
			QString file = TypeImage;
			UnitTextures[n].loadFromFile(file.toStdString());
		};

		UnitSprite.setTexture(UnitTextures[0]);
}

GameImage::~GameImage()
{
}

void GameImage::IterateAnimation()
{
	{
		CurrentFrame++;
		if (CurrentFrame == 11)
			CurrentFrame = 0;

		UnitSprite.setTexture(this->UnitTextures[CurrentFrame]);
	}
}


void GameImage::SetPositionImage(int x, int y)
{
		UnitSprite.setPosition(x, y);
}

void GameImage::SetDiretionMoving(int direction)
{

}
