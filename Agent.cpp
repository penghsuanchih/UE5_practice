#include "Agent.h"
#include "KiLiGameMode.h"
#include "AgentsManager.h"
#include "AgentPlayerController.h"
#include "Projectile.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

AAgent::AAgent()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	m_weapon.m_pComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("equip_weapon"));

	static ConstructorHelpers::FObjectFinder<UMaterial> decalMaterialAsset(TEXT("Material'/Game/Cursor/M_Order_Decal.M_Order_Decal'"));
	DecalComponent = CreateDefaultSubobject<UDecalComponent>("OrderCursorToWorld");
	if (decalMaterialAsset.Succeeded())
	{
		DecalComponent->SetDecalMaterial(decalMaterialAsset.Object);
		DecalComponent->DecalSize = FVector(16.0f, 72.0f, 72.0f);
		DecalComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
		DecalComponent->SetupAttachment(RootComponent);
		DecalComponent->SetHiddenInGame(true);
	}

	bUseControllerRotationYaw = true;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	maxHP = 100;
	m_pTargetEnemy = nullptr; 
	m_pSkillManager = nullptr;
}

void AAgent::BeginPlay()
{
	Super::BeginPlay();

	AKiLiGameMode* pGameMode = Cast<AKiLiGameMode>(GetWorld()->GetAuthGameMode());
	m_pAgentsManager = pGameMode->GetAgentsManager();
	m_pPlayerController = Cast<AAgentPlayerController>(GetController());
	m_pSkillManager = pGameMode->GetSkillManager();
 
	m_data.curHP = m_data.targetHP = maxHP;
}

void AAgent::Tick(float deltaSeconds)
{
    Super::Tick(deltaSeconds);

	if (!m_pAgentsManager)
	{
		AKiLiGameMode* pGameMode = Cast<AKiLiGameMode>(GetWorld()->GetAuthGameMode());
		m_pAgentsManager = pGameMode->GetAgentsManager();
	}

	if (m_data.animActionType == AGENT_ANIM_ACTION_TYPE::DISAPEAR) return;
	else if (m_data.animActionType == AGENT_ANIM_ACTION_TYPE::DESTROY)
	{
		m_data.animActionType = AGENT_ANIM_ACTION_TYPE::DISAPEAR;
		Destroy();
		return;
	}
	else if (m_data.animActionType == AGENT_ANIM_ACTION_TYPE::ATTACK) {}
	else if (m_data.nextAnimActionType == AGENT_ANIM_ACTION_TYPE::MOVE)
	{
		MoveTo(m_data.vTargetPos);
		m_data.nextAnimActionType = AGENT_ANIM_ACTION_TYPE::IDLE;
	}
	else if (m_data.animActionType == AGENT_ANIM_ACTION_TYPE::ATTACK_READY)
	{
		if (AttackRotate(deltaSeconds) && m_data.attackLockTime <= 0)
			StartAttack();
	}
	//else if (m_pTargetEnemy)
	//{
	//	if (m_data.attackLockTime <= 0)
	//		AttackTo(m_pTargetEnemy);
	//}

	if (m_data.attackLockTime > 0)
		m_data.attackLockTime -= deltaSeconds;

	UpdateDecalComponentState();
	m_data.Update(deltaSeconds);
}

void AAgent::MoveTo(FVector vPos)
{
	m_pTargetEnemy = nullptr;
	if (m_data.animActionType == AGENT_ANIM_ACTION_TYPE::DEATH) {}
	else if (m_data.animActionType == AGENT_ANIM_ACTION_TYPE::IDLE) 
	{
		m_data.animActionType = AGENT_ANIM_ACTION_TYPE::MOVE;
		if (m_pPlayerController)
			m_pPlayerController->MoveTo(vPos);
	}
	else
	{
		m_data.vTargetPos = vPos;
		m_data.nextAnimActionType = AGENT_ANIM_ACTION_TYPE::MOVE;
	}
}

void AAgent::UpdateDecalComponentState()
{
	DecalComponent->SetHiddenInGame(!m_data.bSelectedState);
}

void AAgent::OnNotifyAnimEnd()
{
	if (m_data.animActionType == AGENT_ANIM_ACTION_TYPE::DESTROY) {}
	else if (m_data.animActionType == AGENT_ANIM_ACTION_TYPE::DEATH)
	{
		m_data.animActionType = AGENT_ANIM_ACTION_TYPE::DESTROY;
		OnDestroy();
	}
	else 
	{
		if (!m_pSkillManager->OnAttackEnd(this, m_data.curSkillType))
		{
			m_data.curSkillType = SKILL_TYPE::NONE;
			m_data.animActionType = AGENT_ANIM_ACTION_TYPE::IDLE;
			m_data.attackLockTime = HERO_ATTACK_LOCK_TIME;
		}
	}
}

void AAgent::OnNotifyMoveStop()
{
	if (m_data.animActionType == AGENT_ANIM_ACTION_TYPE::MOVE)
		m_data.animActionType = AGENT_ANIM_ACTION_TYPE::IDLE;
}

void AAgent::OnDeath()
{
	m_data.targetHP = 0;
	m_pPlayerController->StopMovement();
	m_data.animActionType = AGENT_ANIM_ACTION_TYPE::DEATH;
 	m_pAgentsManager->OnNitifyAgentDeath(this);
}

void AAgent::OnDestroy()
{
	Destroy();
}

void AAgent::AttackTo(AAgent* pHeroCharacter)
{
	m_data.animActionType = AGENT_ANIM_ACTION_TYPE::ATTACK_READY;
	m_pTargetEnemy = pHeroCharacter;
}

void AAgent::TakeAttackDamage(float damage, AAgent* pEnemy, bool hurtOverlay, FVector vPos)
{
	if (IsEnemy(pEnemy) && m_data.targetHP > 0)
	{
		AKiLiGameMode* pGameMode = Cast<AKiLiGameMode>(GetWorld()->GetAuthGameMode());
		pGameMode->SpawnEffectParticle(VISIBLE_EFFECT_TYPE::BLOOD, vPos);
		if (!hurtOverlay && m_data.animActionType == AGENT_ANIM_ACTION_TYPE::HURT) {}
		else
		{
			m_pPlayerController->StopMovement();
			m_data.AddShowHurtData(damage);
			m_data.targetHP -= damage;
			if (m_data.targetHP <= 0)
				OnDeath();
			//else
			//	m_data.animActionType = AGENT_ANIM_ACTION_TYPE::HURT;
		}
	}
}

bool AAgent::AttackRotate(float deltaSeconds)
{
	if (m_pTargetEnemy)
	{
		float RotateYaw = HERO_ATTACK_ROTATE_SPEED * deltaSeconds;
		float yawWant = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), m_pTargetEnemy->GetActorLocation()).Yaw;
		float yawDelta = yawWant - GetActorRotation().Yaw;
		int dir = FMath::Sign(yawDelta);
		if (abs(yawDelta) > RotateYaw)
		{
			AddActorWorldRotation(FRotator(0, dir * RotateYaw, 0));
			return false;
		}
		else
			SetActorRotation(FRotator(0, yawWant, 0));
	}

	return true;
}

void AAgent::OnSwitchSkill(int index)
{
	m_data.nextSkillIndex = index;
}

bool AAgent::IsEnemy(AAgent* pAgent)
{
	return (m_data.groupType != pAgent->GetGroupType())? true : false;
}

void AAgent::UpdateRenderCustomDepthState(FVector vCameraPos)
{
	if (m_data.targetHP > 0)
	{
		FHitResult hitResult;
		FCollisionQueryParams params;
		FVector vPos = GetActorLocation() + FVector(0, 0, 50);
		params.AddIgnoredActor(this);
		if (CheckLineDetectState(vCameraPos))
		{
			GetMesh()->SetRenderCustomDepth(true);
			GetMesh()->SetCustomDepthStencilValue(2);
			return;
		}
	}

	GetMesh()->SetRenderCustomDepth(false);
}

bool AAgent::CheckLineDetectState(FVector vCameraPos)
{
	FHitResult hitResult;
	FCollisionQueryParams params;
	FVector vPos = GetActorLocation() + FVector(0, 0, 50);
	params.AddIgnoredActor(this);
	if (GetWorld()->SweepSingleByChannel(hitResult, vCameraPos, vPos, FQuat::Identity,
		ECollisionChannel::ECC_Pawn, FCollisionShape::MakeSphere(10), params))
		return true;

	return false;
}

void AAgent::OnNotifiedAgentDied(AAgent* pAgent)
{
	if (pAgent == m_pTargetEnemy)
		m_pTargetEnemy = nullptr;
}
 
void AAgent::SetDefaultWeaponItem(FName name)
{
	m_weapon.defaultName = name;
	SetWeapon(name);
}

FName AAgent::GetWeaponSocketName()
{
	return GetWeaponSocketName(m_weapon.name);
}

FName AAgent::GetWeaponSocketName(FName name)
{
	return (name == "bow")? "WEAPON_L" : "WEAPON_R";
}
 
USkeletalMesh* AAgent::GetWeponMesh()
{
	return m_weapon.m_pComp->SkeletalMesh;
}
 
void AAgent::OnAttackBegin(SKILL_TYPE skillType)
{
	if (m_data.curSkillType == skillType)
	m_pSkillManager->OnAttackBegin(this, skillType);
}

FVector AAgent::GetWeaponAttackSocketPosition(FName name)
{
	return m_weapon.m_pComp->GetSocketLocation(name);
}

void AAgent::AddHP(long hp)
{
	m_data.targetHP += hp;
	if (m_data.targetHP > maxHP)
		m_data.targetHP = maxHP;
}