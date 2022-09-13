#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SkillData.h"
#include "AgentAttackNotifyState.generated.h"
 

UCLASS()
class UAgentAttackNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Info")
      SKILL_TYPE  skillType;
};
