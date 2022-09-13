 #include "AgentAttackNotifyState.h"
#include "Agent.h"
#include "Components/SkeletalMeshComponent.h"
 
void UAgentAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration);

    if (MeshComp && MeshComp->GetOwner())
    {
        AAgent* pAgent = Cast<AAgent>(MeshComp->GetOwner());
		if (pAgent)
            pAgent->OnAttackBegin(skillType);
    }
}