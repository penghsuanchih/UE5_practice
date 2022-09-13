 #include "AgentAnimNotifyState.h"
#include "Agent.h"
#include "Components/SkeletalMeshComponent.h"


void UAgentAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::NotifyEnd(MeshComp, Animation);

    if (MeshComp && MeshComp->GetOwner())
    { 
        AAgent* pAgent = Cast<AAgent>(MeshComp->GetOwner());
        if (pAgent)
            pAgent->OnNotifyAnimEnd();
    }
}