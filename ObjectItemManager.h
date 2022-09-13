#pragma once

#include "ObjectItem.h"
#include "ObjectItemManager.generated.h"

UTexture2D* LoadTextureFromPath(const FString& Path);
UStaticMesh* LoadStaticMeshFromPath(const FString& Path);
USkeletalMesh* LoadSkeletalMeshFromPath(const FString& Path);

UCLASS()
class AObjectItemManager : public AActor
{
	GENERATED_BODY()

public:
	AObjectItemManager();
	void AddData(FName id, FString szPicName, FString szMeshName, OBJECT_ITEM_TYPE type);
	ObjectItem* GetObjectItem(FName id);

private:
	TMap<FName, ObjectItem> m_objectItemMap;
};
