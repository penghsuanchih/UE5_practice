#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AgentAnimNotifyState.generated.h"
 

UCLASS()
class UAgentAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);


};
