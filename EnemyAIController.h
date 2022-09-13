#pragma once

#include "CoreMinimal.h"
#include "AgentPlayerController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

 
class AEnemyAgent;
UCLASS()
class AEnemyAIController : public AAgentPlayerController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Enemy)
		class AEnemyAgent* EnemyCharacter;

	UPROPERTY(VisibleDefaultsOnly)
		class UAIPerceptionComponent* Perception;

	UPROPERTY(VisibleDefaultsOnly)
		class UAISenseConfig_Sight* Sight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
		float SightRadius = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
		float LoseSightRadius = SightRadius + 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
		float SightMaxAge = 30.f;

	FTimerHandle RoamIdleTimer;
	FTimerHandle ChaseIdleTimer;

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* EnemyPawn) override;

	UFUNCTION()
		void OnTargetPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
		void OnComponentEnterDamageCollider(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentLeaveDamageCollider(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	void IdleOrRoam();
	void Chase(class AAgent* PlayerCharacter);
	void StopChase();
};
