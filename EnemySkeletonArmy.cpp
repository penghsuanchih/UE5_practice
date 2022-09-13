#include "EnemySkeletonArmy.h"
#include "EnemyAIController.h"
#include "KiLiGameMode.h"
#include "SkillManager.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemySkeletonArmy::AEnemySkeletonArmy()
{
	DamageCollider->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	m_data.groupType = AGENT_GROUP_TYPE::ENEMY;
}
 
void AEnemySkeletonArmy::BeginPlay()
{
	Super::BeginPlay();

	SetWeapon("sword2");
}

void AEnemySkeletonArmy::StartAttack()
{
	m_pSkillManager->UseSkill(this, SKILL_TYPE::SWORD_ATTACK);
}

void AEnemySkeletonArmy::OnDestroy()
{
 	AKiLiGameMode* pGameMode = Cast<AKiLiGameMode>(GetWorld()->GetAuthGameMode());
 	pGameMode->SpawnDropObject(GetActorLocation() + FVector(0, 0, -85), "flowerpot");
}