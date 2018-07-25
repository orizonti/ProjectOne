#pragma once
#include "HeaderAndStructures.h"

class GameImage
{
public:
public:
	GameImage(std::string path_to_Image = "");
	~GameImage();
	int CurrentFrame = 0;

	sf::Image hero;
	sf::Texture textureHero;
	sf::Sprite spriteHero;

	sf::Texture textureHeroMove[12];

	void SetPositionImage(int x, int y);

	void SetDiretionMoving(int direction);
	void IterateAnimation();
};

