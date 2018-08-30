#include "GameImageClass.h"



GameImage::GameImage()
{


}


GameImage::~GameImage()
{
}

void GameImage::LinkAnimationSet(std::shared_ptr<ClassAnimationSet> Animation)
{
	AnimationSet = Animation;
}

void GameImage::IterateAnimation(Direction Dir)
{
	if (AnimationSet == NULL)
		return;

	if (Dir != CurrentDir)
	{
		CurrentFrame = 0;
		Dir = CurrentDir;
	}

		if (CurrentFrame == 18)
			CurrentFrame = 0;

		this->CurrentSprite.setTexture(AnimationSet->GetTexture(Dir,CurrentFrame));
		this->CurrentSprite1.setTexture(AnimationSet->GetTexture(Dir,CurrentFrame));
		this->CurrentSprite2.setTexture(AnimationSet->GetTexture(Dir,CurrentFrame));
		this->CurrentSprite3.setTexture(AnimationSet->GetTexture(Dir,CurrentFrame));

		CurrentFrame++;

}

sf::Texture& ClassAnimationSet::GetTexture(Direction Dir, int Frame)
{
	return *TexturesSet[Dir][Frame].get();
}

void GameImage::SetPositionImage(QPair<int, int> Coord)
{
		CurrentSprite.setPosition(Coord.first - 10,Coord.second-5);
		CurrentSprite1.setPosition(Coord.first - 240,Coord.second - 5);
		CurrentSprite2.setPosition(Coord.first - 120 ,Coord.second + 60);
		CurrentSprite3.setPosition(Coord.first - 120,Coord.second -65);
}
void GameImage::SetPositionImage(int x, int y)
{
		CurrentSprite.setPosition( x - 10  ,y - 5);
		CurrentSprite1.setPosition(x - 240 ,y - 5);
		CurrentSprite2.setPosition(x - 120 ,y + 60);
		CurrentSprite3.setPosition(x - 120 ,y - 65);
}


void GameImage::SetDiretionMoving(Direction Dir)
{
	this->CurrentDir = Dir;
}

void ClassAnimationSet::UploadAnimation(QString Name, QString SetDir)
{
	qDebug() << "UPLOAD ANIMATION - " << SetDir <<"NAME - " <<Name << "FRAME COUNT - " << FrameCount << "SET COUNT - " << SetCount;

		QString FileName;
		for (int set_number = 1; set_number <= SetCount; set_number++)
		{
			QVector<std::shared_ptr<sf::Texture>> NewTexturesGroup;

		    QString PathToSet = SetDir + Name + QString("/%1/").arg(set_number) + "320/";
			qDebug() << "         LOAD SET NUMBER - " << set_number;
			for (int frame_number = 1; frame_number <= FrameCount; frame_number++)
			{
				auto NewTexture = std::make_shared<sf::Texture>(sf::Texture());
				NewTexturesGroup.append(NewTexture);

				if (frame_number < 10)
					FileName = QString("000%1.png").arg(frame_number);
				else
					FileName = QString("00%1.png").arg(frame_number);

				qDebug() << "LOADE IMAGE - " << FileName;
				QString Full_Path = PathToSet + FileName;

				NewTexturesGroup.last()->loadFromFile(Full_Path.toStdString());
			}
			TexturesSet.append(NewTexturesGroup);
		}
}


void GameImage::SetTexture(sf::Texture& Texture)
{
	this->CurrentSprite.setTexture(Texture);
	this->CurrentSprite1.setTexture(Texture);
	this->CurrentSprite2.setTexture(Texture);
	this->CurrentSprite3.setTexture(Texture);
}
