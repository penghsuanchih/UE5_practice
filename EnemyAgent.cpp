#include "EnemyAgent.h"
#include "EnemyAIController.h"
#include "KiLiGameMode.h"
#include "SkillManager.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyAgent::AEnemyAgent()
{
	DamageCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("EnemyDamageCollider"));
	DamageCollider->SetRelativeLocation(FVector(70.f, 0.f, 40.f));
	DamageCollider->SetBoxExtent(FVector(30.f, 30.f, 20.f));
	DamageCollider->SetGenerateOverlapEvents(true);
	DamageCollider->SetCollisionProfileName(TEXT("Trigger"));
	DamageCollider->SetupAttachment(RootComponent);

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	m_data.groupType = AGENT_GROUP_TYPE::ENEMY;
}

void AEnemyAgent::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
}

void AEnemyAgent::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	if (m_data.animActionType == AGENT_ANIM_ACTION_TYPE::DEATH) {}
	else if (m_data.animActionType == AGENT_ANIM_ACTION_TYPE::ATTACK)
		m_pSkillManager->OnUpdate(this);
}

void AEnemyAgent::SetWeapon(FName name)
{
	ObjectItem* pItem = nullptr;
	if (m_pAgentsManager)
		pItem = m_pAgentsManager->GetObjectItem(name);
	else
	{
		AKiLiGameMode* pGameMode = Cast<AKiLiGameMode>(GetWorld()->GetAuthGameMode());
		AObjectItemManager* pObjectItemManager = pGameMode->GetObjectItemManager();
		pItem = pObjectItemManager->GetObjectItem(name);
	}

	if (pItem)
	{
		m_weapon.name = name;
		m_weapon.m_pComp->SetSkeletalMesh(pItem->skeletalMesh);
		m_weapon.m_pComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, HERO_WEAPON_SOCKET);
	}
}

void AEnemyAgent::ToIdleState()
{
	PreviousAnimActionType = m_data.animActionType;
	m_data.animActionType = AGENT_ANIM_ACTION_TYPE::IDLE;
}

void AEnemyAgent::ToRoamState()
{
	PreviousAnimActionType = m_data.animActionType;
	m_data.animActionType = AGENT_ANIM_ACTION_TYPE::ROAM;

	UCharacterMovementComponent* pEnemyMovement = GetCharacterMovement();
	if (pEnemyMovement != nullptr)
		pEnemyMovement->MaxWalkSpeed = RoamSpeed;
}

void AEnemyAgent::ToChaseState()
{
	PreviousAnimActionType = m_data.animActionType;
	m_data.animActionType = AGENT_ANIM_ACTION_TYPE::CHASE;

	UCharacterMovementComponent* pEnemyMovement = GetCharacterMovement();
	if (pEnemyMovement != nullptr)
		pEnemyMovement->MaxWalkSpeed = ChaseSpeed;
}

void AEnemyAgent::ToAttackState(AAgent* pAgent)
{
	PreviousAnimActionType = m_data.animActionType;
	SetTargetEnemy(pAgent);
	StartAttack();
}

void AEnemyAgent::ToDeadState()
{
	PreviousAnimActionType = m_data.animActionType;
	m_data.animActionType = AGENT_ANIM_ACTION_TYPE::DEATH;
}

void AEnemyAgent::StartAttack()
{
	m_pSkillManager->UseSkill(this, SKILL_TYPE::ZOMBIE_ATTACK);
}
 