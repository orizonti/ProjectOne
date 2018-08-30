#include "AnimationSet.h"




sf::Texture& AnimationSet::GetTexture(Direction Dir, int Frame)
{
	return *TexturesSet[Dir][Frame].get();
}
void AnimationSet::UploadAnimation(QString Name, QString SetDir)
{
	//qDebug() << "UPLOAD ANIMATION - " << SetDir <<"NAME - " <<Name << "FRAME COUNT - " << FrameCount << "SET COUNT - " << SetCount;

		QString FileName;
		for (int set_number = 1; set_number <= SetCount; set_number++)
		{
			QVector<std::shared_ptr<sf::Texture>> NewTexturesGroup;

		    QString PathToSet = SetDir + Name + QString("/%1/").arg(set_number) + "320/";
			//qDebug() << "         LOAD SET NUMBER - " << set_number;
			for (int frame_number = 1; frame_number <= FrameCount; frame_number++)
			{
				auto NewTexture = std::make_shared<sf::Texture>(sf::Texture());
				NewTexturesGroup.append(NewTexture);

				if (frame_number < 10)
					FileName = QString("000%1.png").arg(frame_number);
				else
					FileName = QString("00%1.png").arg(frame_number);

				//qDebug() << "LOADE IMAGE - " << FileName;
				QString Full_Path = PathToSet + FileName;

				NewTexturesGroup.last()->loadFromFile(Full_Path.toStdString());
			}
			TexturesSet.append(NewTexturesGroup);
		}
}
