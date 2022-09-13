#include "EnemyAIController.h"
#include "EnemyZombie.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"

 
AEnemyAIController::AEnemyAIController()
{
 	Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("EnemyPerception"));
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("EnemySight"));
	Sight->SightRadius = SightRadius;
	Sight->LoseSightRadius = SightRadius;
	Sight->SetMaxAge(SightMaxAge);
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(Sight->GetSenseImplementation());
	Perception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdate);

	bStartAILogicOnPossess = true; 
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	EnemyCharacter->ToIdleState();
}

void AEnemyAIController::OnPossess(APawn* pEnemyPawn)
{
	Super::OnPossess(pEnemyPawn);

	EnemyCharacter = Cast<AEnemyAgent>(pEnemyPawn);
	EnemyCharacter->DamageCollider->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAIController::OnComponentEnterDamageCollider);
	EnemyCharacter->DamageCollider->OnComponentEndOverlap.AddDynamic(this, &AEnemyAIController::OnComponentLeaveDamageCollider);
}

void AEnemyAIController::OnTargetPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	TArray<AActor*> PerceivedActors;
	Perception->GetCurrentlyPerceivedActors(TSubclassOf<UAISense_Sight>(), PerceivedActors);

	bool bPlayerIsInSight = PerceivedActors.Contains(Actor);
	if (!bPlayerIsInSight)
	{
		StopChase();
		return;
	}

	if (EnemyCharacter->GetAnimActionState() == AGENT_ANIM_ACTION_TYPE::ATTACK ||
		EnemyCharacter->GetAnimActionState() == AGENT_ANIM_ACTION_TYPE::HURT) {}
	else if (EnemyCharacter->GetAnimActionState() == AGENT_ANIM_ACTION_TYPE::IDLE)
	{
		AAgent* pAgent = Cast<AAgent>(Actor);
		if (pAgent == nullptr || !EnemyCharacter->IsEnemy(pAgent)) return;

		Chase(pAgent);
	}
} 
 
void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (EnemyCharacter->GetAnimActionState() == AGENT_ANIM_ACTION_TYPE::CHASE)
		StopChase();
}

void AEnemyAIController::Chase(AAgent* PlayerCharacter)
{
 	EnemyCharacter->ToChaseState();
 	MoveToActor(PlayerCharacter);
}
 
void AEnemyAIController::StopChase()
{
	StopMovement();
 
	EnemyCharacter->ToIdleState();
	//if (EnemyCharacter->AfterChaseDelay > 0.f)
	//{
	//	UWorld* World = GetWorld();
	//	if (World != nullptr)
	//	{
//			EnemyCharacter->ToIdleState();
//			GetWorld()->GetTimerManager().SetTimer(ChaseIdleTimer, this, &AEnemyAIController::IdleOrRoam, EnemyCharacter->AfterChaseDelay, false);
	//	}
	//}
 	//else
 	//	IdleOrRoam();
}

void AEnemyAIController::IdleOrRoam()
{
    //if (EnemyCharacter->bCanRoam)
	//{
	//	if (EnemyCharacter->PreviousAnimActionType == AGENT_ANIM_ACTION_TYPE::CHASE)
	//		EnemyCharacter->StartLocation = EnemyCharacter->GetActorLocation();

 	//	Roam();
	//}
 //	else 
 		EnemyCharacter->ToIdleState();
}
 
void AEnemyAIController::OnComponentEnterDamageCollider(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
 	AAgent* pAgent = Cast<AAgent>(OtherActor);
 	if (pAgent == nullptr || !EnemyCharacter->IsEnemy(pAgent)) return;

 	EnemyCharacter->ToAttackState(pAgent);
}

void AEnemyAIController::OnComponentLeaveDamageCollider(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  	AAgent* pAgent = Cast<AAgent>(OtherActor);
 	if (pAgent == nullptr || !EnemyCharacter->IsEnemy(pAgent)) return;

	if (EnemyCharacter->GetAnimActionState() == AGENT_ANIM_ACTION_TYPE::IDLE)
 		Chase(pAgent);
}
