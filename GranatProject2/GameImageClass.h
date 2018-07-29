#pragma once
#include "HeaderAndStructures.h"

class GameImage
{
public:
public:
	GameImage();
	~GameImage();
	int CurrentFrame = 0;

	sf::Sprite UnitSprite;
	sf::Texture UnitTextures[12];

	void SetPositionImage(int x, int y);

	void SetDiretionMoving(int direction);
	void IterateAnimation();

	void SetImage(QString TypeUnit);
};

