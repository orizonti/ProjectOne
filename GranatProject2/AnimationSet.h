#pragma once
#include <HeaderAndStructures.h>

class AnimationSet
{
public:
	AnimationSet(int CountFrame, int CountSet)
	{
		FrameCount = CountFrame;
		SetCount = CountSet;
	}
	int FrameCount = 20;
	int SetCount   = 8;

	void UploadAnimation(QString Name, QString SetDir);

	
	sf::Texture& GetTexture(Direction Dir, int Frame);
	QVector<QVector< std::shared_ptr<sf::Texture> >> TexturesSet;


};
