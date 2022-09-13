#include "HeroPlayerController.h"
#include "Agent.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"

AHeroPlayerController::AHeroPlayerController()
{
}

void AHeroPlayerController::MoveTo(FVector vPos)
{
	StopMovement();
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, vPos);
}

void AHeroPlayerController::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	AAgent* pAgent = Cast<AAgent>(GetCharacter());
	pAgent->OnNotifyMoveStop();
}