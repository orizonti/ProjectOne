#pragma once
#include "GameImageClass.h"
#include "HeaderAndStructures.h"


class AnimationSetContainer
{
public:
	void UploadAnimationSets(QString PathToAnimation);

	QMap<int,   std::shared_ptr<ClassAnimationSet>> AnimationSetsToUnits;
	QMap<int, std::shared_ptr<ClassAnimationSet>> AnimationSetsToMapObjects;
	QMap<QString, int> UnitsType;
	QMap<QString, int> ObjectType;

	ClassAnimationSet &GetUnitAnimationSet(TypeUnit Unit);
	ClassAnimationSet &GetObjectAnimationSet(TypeObject Unit);
};