#pragma once
#include "GameImageClass.h"
#include "HeaderAndStructures.h"
#include "AnimationSet.h"


class AnimationSetContainer
{
public:
	void UploadAnimationSets(QString PathToAnimation);

	QMap<int,   std::shared_ptr<AnimationSet>> AnimationSetsToUnits;
	QMap<int,   std::shared_ptr<AnimationSet>> AnimationSetsToMapObjects;
	QMap<QString, int> UnitsType;
	QMap<QString, int> ObjectType;

	std::shared_ptr<AnimationSet> &GetUnitAnimationSet(int Unit);
	std::shared_ptr<AnimationSet> &GetUnitAnimationSet(QString TypeUnit);
	std::shared_ptr<AnimationSet> &GetObjectAnimationSet(int Object);
	std::shared_ptr<AnimationSet> &GetObjectAnimationSet(QString TypeObject);
};