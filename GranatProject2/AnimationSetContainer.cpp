#include "AnimationSetContainer.h"

ClassAnimationSet &AnimationSetContainer::GetUnitAnimationSet(TypeUnit Unit)
{
	return *this->AnimationSetsToUnits[Unit].get();
}

ClassAnimationSet &AnimationSetContainer::GetObjectAnimationSet(TypeObject Object)
{
	return *this->AnimationSetsToMapObjects[Object].get();
}

void AnimationSetContainer::UploadAnimationSets(QString PathToAnimation)
{
	QDir AnimationDirectory(PathToAnimation);
	qDebug() << "UPLOAD ANIMATION FROM DIRECTORY - " << PathToAnimation;
	auto FileInfo = AnimationDirectory.entryInfoList();

	for (QFileInfo& entity : FileInfo)
	{
	    qDebug() << "ANIMATION DIR CONTAINING - " << entity.baseName();
		if (!entity.baseName().isEmpty())
		{
	    qDebug() << "APPEND - " << entity.baseName();
		UnitsType.insert(entity.baseName(), UnitsType.size() + 1);
		}

	}


	for (QString NameUnit : UnitsType.keys())
	{
		auto NewAnimationSet = std::make_shared<ClassAnimationSet>(ClassAnimationSet(20, 8));
		NewAnimationSet->UploadAnimation(NameUnit, PathToAnimation);
	}
		

}
