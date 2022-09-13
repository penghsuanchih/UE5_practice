#pragma once

#include "CoreMinimal.h"
#include "ObjectItem.h"
#include "Engine/StaticMeshActor.h"
#include "DropObject.generated.h"


UCLASS()
class ADropObject : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:	
	ADropObject();
	void SetObjectItem(FName name, ObjectItem* pObjectItem);
	FName GetObjectName() {return m_szName;}
	void SetRenderCustomDepthState(bool bShowState);
	
private:
	UStaticMeshComponent* m_pMeshComp;
	ObjectItem* m_pObjectItem;
	FName m_szName;
 
};
