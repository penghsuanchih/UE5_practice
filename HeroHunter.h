#pragma once

#include "CoreMinimal.h"
#include "HeroAgent.h"
#include "EquipData.h"
#include "HeroHunter.generated.h"
 

UCLASS(Blueprintable)
class  AHeroHunter : public AHeroAgent
{
	GENERATED_BODY()

public:
	AHeroHunter();
	virtual void BeginPlay() override;
 
protected:
 
	void StartAttack();
 
};

