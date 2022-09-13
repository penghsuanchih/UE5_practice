#pragma once

#include "CoreMinimal.h"
#include "HeroAgent.h"
#include "EquipData.h"
#include "HeroKnight.generated.h"
 

UCLASS(Blueprintable)
class  AHeroKnight : public AHeroAgent
{
	GENERATED_BODY()

public:
	AHeroKnight();
	virtual void BeginPlay() override;
 
protected:
 
	void StartAttack();
 
};

