#pragma once

#include "CoreMinimal.h"
#include "Agent.h"
#include "EquipData.h"
#include "HeroAgent.generated.h"
 

UCLASS(Blueprintable)
class AHeroAgent : public AAgent
{
	GENERATED_BODY()

public:
	AHeroAgent();
	virtual void BeginPlay() override;
	void SetWeapon(FName name);
	USkeletalMesh* GetEquipMesh(EQUIP_ITEM_TYPE type);
	void SetEquipItem(EQUIP_ITEM_TYPE type, FName name);

protected:

	FVector GetWeaponSocketPosition();
	void SetDefaultEquipItem(EQUIP_ITEM_TYPE type, FName name);
	EquipData m_equipData[int(EQUIP_ITEM_TYPE::COUNT)];
	UAnimBlueprintGeneratedClass* m_pAnimClass;
};

