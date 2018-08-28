#include "GameImageClass.h"



GameImage::GameImage()
{


}


GameImage::~GameImage()
{
}

void GameImage::IterateAnimation(Direction Dir)
{

	if (Dir != CurrentDir)
	{
		CurrentFrame = 0;
		Dir = CurrentDir;
	}

		if (CurrentFrame == 20)
			CurrentFrame = 0;

		this->CurrentSprite.setTexture(AnimationSet->GetTexture(Dir,CurrentFrame));

		CurrentFrame++;

}

sf::Texture& ClassAnimationSet::GetTexture(Direction Dir, int Frame)
{
	return *TexturesSet[Dir][Frame].get();
}

void GameImage::SetPositionImage(int x, int y)
{
		CurrentSprite.setPosition(x, y);
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

		    QString PathToSet = SetDir + Name + QString("/%1/").arg(set_number) + "256/";
			qDebug() << "         LOAD SET NUMBER - " << set_number;
			for (int frame_number = 1; frame_number <= FrameCount; frame_number++)
			{
				auto NewTexture = std::make_shared<sf::Texture>(sf::Texture());
				NewTexturesGroup.append(NewTexture);

				if (frame_number < 10)
					FileName = QString("NEW_ANIMATION") + QString("000%1.png").arg(frame_number);
				else
					FileName = QString("NEW_ANIMATION") + QString("00%1.png").arg(frame_number);

				qDebug() << "LOADE IMAGE - " << FileName;
				QString Full_Path = PathToSet + FileName;

				NewTexturesGroup.last()->loadFromFile(Full_Path.toStdString());
			}
			TexturesSet.append(NewTexturesGroup);
		}
}

