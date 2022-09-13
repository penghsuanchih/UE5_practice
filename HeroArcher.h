#pragma once

#include "CoreMinimal.h"
#include "HeroAgent.h"
#include "EquipData.h"
#include "HeroArcher.generated.h"
 

UCLASS(Blueprintable)
class  AHeroArcher : public AHeroAgent
{
	GENERATED_BODY()

public:
	AHeroArcher();
	virtual void BeginPlay() override;
 
protected:
 
	void StartAttack();
 
};

