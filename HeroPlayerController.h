#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "AgentPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "HeroPlayerController.generated.h"

class AAgent;
class UNiagaraSystem;
UCLASS()
class AHeroPlayerController : public AAgentPlayerController
{
	GENERATED_BODY()

public:
	AHeroPlayerController();
	virtual void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result) override;

	void MoveTo(FVector vPos);

private:
 

};


