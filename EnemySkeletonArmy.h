#pragma once

#include "CoreMinimal.h"
#include "EnemyAgent.h"
#include "EnemySkeletonArmy.generated.h"
 

UCLASS(Blueprintable)
class AEnemySkeletonArmy : public AEnemyAgent
{
	GENERATED_BODY()

public:
	AEnemySkeletonArmy();
	virtual void BeginPlay() override;
	void StartAttack();
	void OnDestroy();
};

