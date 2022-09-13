#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "AIController.h"
#include "GameFramework/PlayerController.h"
#include "AgentPlayerController.generated.h"

class AAgent;
class UNiagaraSystem;
UCLASS()
class AAgentPlayerController : public AAIController
{
	GENERATED_BODY()

public:
	AAgentPlayerController();
	virtual void MoveTo(FVector vPos) {}

private:
 

};


