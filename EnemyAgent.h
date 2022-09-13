#pragma once

#include "CoreMinimal.h"
#include "Agent.h"
#include "EnemyAgent.generated.h"
 

UCLASS(Blueprintable)
class AEnemyAgent : public AAgent
{
	GENERATED_BODY()

public:
	AEnemyAgent();

	AGENT_ANIM_ACTION_TYPE PreviousAnimActionType = m_data.animActionType;
 
	UPROPERTY(VisibleDefaultsOnly);
	class UBoxComponent* DamageCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RoamState)
		FVector StartLocation;

	// Indicates whether the ZombieCharacter should be able to roam or not.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoamState)
		bool bCanRoam = true;

	// The max speed of the ZombieCharacter in the ROAM state.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoamState)
		float RoamSpeed = 50.f;

	// The area around its spawn point that the ZombieCharacter can roam.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoamState)
		float RoamRadius = 400.f;

	// The amount of time to pause in between `Roam` calls. If set to 0 there will
	// be no delay.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoamState)
		float RoamDelay = 3.f;

	// The max speed of the ZombieCharacter in the CHASE state.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChaseState)
		float ChaseSpeed = 300.f;

	// The amount of delay after a chase after which the ZombieCharacter will
	// resume to roam. This is to help break up an awkward transition from chasing
	// straight back to roaming.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChaseState)
		float AfterChaseDelay = 3.f;

	// The amount of seconds long that the zombie dying animation is. This is used with
	// the `SecondsAfterDeathBeforeDestroy` variable to make sure that the dying animation
	// plays out fully before the ZombieCharacter is destroyed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DyingState)
		float DyingAnimationLengthInSeconds = 3.f;

	// The amount of time after the ZombieCharacter dies that they will destroy. A value
	// of 0 means that the ZombieCharacter will be destroyed immediately after the dying
	// animation plays. A value below 0 means that the ZombieCharacter will never be destroyed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DyingState)
		float SecondsAfterDeathBeforeDestroy = 5.f;

protected:
	FTimerHandle DeathAnimationTimer;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaSeconds) override;
	void AfterDeathAnimationFinished();
	void StartAttack();
	void SetWeapon(FName name);

public:
	void ToIdleState();
	void ToRoamState();
	void ToChaseState();
	void ToAttackState(AAgent* pAgent);
	void ToDeadState();
};

