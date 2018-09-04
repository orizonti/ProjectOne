#include "AnimationSetContainer.h"

std::shared_ptr<AnimationSet> &AnimationSetContainer::GetUnitAnimationSet(int Unit)
{
	//qDebug() << "GET UNIT NUMBER - " << Unit;
	return this->AnimationSetsToUnits[Unit];
}

std::shared_ptr<AnimationSet>& AnimationSetContainer::GetUnitAnimationSet(QString TypeUnit)
{
	return this->AnimationSetsToUnits[UnitsType[TypeUnit]];
}

std::shared_ptr<AnimationSet>& AnimationSetContainer::GetObjectAnimationSet(QString TypeObject)
{
	return this->AnimationSetsToUnits[UnitsType[TypeObject]];
}

std::shared_ptr<AnimationSet> &AnimationSetContainer::GetObjectAnimationSet(int Object)
{
	return this->AnimationSetsToMapObjects[Object];
}

void AnimationSetContainer::UploadAnimationSets(QString PathToAnimation)
{
	QDir AnimationDirectory(PathToAnimation);
	//qDebug() << "UPLOAD ANIMATION FROM DIRECTORY - " << PathToAnimation;
	auto FileInfo = AnimationDirectory.entryInfoList();

	for (QFileInfo& entity : FileInfo)
	{
	   // qDebug() << "ANIMATION DIR CONTAINING - " << entity.baseName();
		if (!entity.baseName().isEmpty())
		{
	   // qDebug() << "APPEND - " << entity.baseName();
		UnitsType.insert(entity.baseName(), UnitsType.size() + 1);
		}

	}


	for (QString NameUnit : UnitsType.keys())
	{
		auto NewAnimationSet = std::make_shared<AnimationSet>(AnimationSet(20, 8));
		NewAnimationSet->UploadAnimation(NameUnit, PathToAnimation);
		AnimationSetsToUnits.insert(AnimationSetsToUnits.size() + 1, NewAnimationSet);
	}
		

}
