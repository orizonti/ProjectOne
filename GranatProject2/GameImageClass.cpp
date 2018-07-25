#include "GameImageClass.h"



GameImage::GameImage(std::string path_to_Image = "")
{
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

		spriteHero.setTexture(this->textureHeroMove[CurrentFrame]);
	}
}


void GameImage::SetPositionImage(int x, int y)
{
		spriteHero.setPosition(x, y);
}

void GameImage::SetDiretionMoving(int direction)
{

}
