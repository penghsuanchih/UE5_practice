#include "Projectile.h"
#include "KiLiGameMode.h"
#include "Setting.h"
#include "Agent.h"
#include "Kismet/GameplayStatics.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	m_fDeltaSeconds = 0.0;
	m_fDelayTime = 0.0;
	m_fSize = 5.0;
	m_bStopMove = false;
	m_bDisappear = false;
	SetMobility(EComponentMobility::Movable);
}
 
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AKiLiGameMode* pGameMode = Cast<AKiLiGameMode>(GetWorld()->GetAuthGameMode());
	m_fDeltaSeconds += DeltaTime;
	if (m_fDelayTime > 0)
	{
		m_fDelayTime -= DeltaTime;
		if (m_fDelayTime <= 0)
			SetActorHiddenInGame(false);
	}
	else if (m_fDeltaSeconds >= BULLET_DISAPPEAR_SPEED)
		Destroy();
	else if (!m_bStopMove && !m_bDisappear)
	{
		FVector vPos = GetActorLocation();
		FVector vTarget = vPos + m_vDir * m_fSpeed * DeltaTime;
		UpdateFire(pGameMode, vPos, vTarget);
	}
}

bool AProjectile::Fire(AAgent* pTarget, FVector vPos, FVector vDir, long damage, float speed, float delayTime, float size)
{
	SetActorLocation(vPos);
	m_vDir = vDir;
	m_pOwner = pTarget;
	m_damage = damage;
	m_fSpeed = speed;
	m_fDelayTime = delayTime;
	m_fSize = size;

	if (m_fDelayTime > 0)
		SetActorHiddenInGame(true);

	return true;
}

void AProjectile::UpdateFire(AKiLiGameMode* pGameMode, FVector vStartPos, FVector vEndPos)
{
	FVector vHitPos;
	AActor* pHitedActor = HitDetect(vStartPos, vEndPos, vHitPos);
	if (pHitedActor)
	{
		FDamageEvent DamageEvent;
		if (pHitedActor->IsA(AAgent::StaticClass()))
		{
			AAgent* pTargetAgent = Cast<AAgent>(pHitedActor);
			{
				pTargetAgent->TakeAttackDamage(m_damage, m_pOwner, true, vHitPos);
				m_bDisappear = true;
			}
		}
		else
			m_bDisappear = true;

		if (m_bDisappear)
		{
			Destroy();
			return;
		}
	}

	FRotator rot = m_vDir.Rotation();
	SetActorLocation(vEndPos);
	SetActorRotation(rot);
}

AActor* AProjectile::HitDetect(FVector vStartPos, FVector vEndPos, FVector& vHitPos)
{
	FHitResult hitResult;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(m_pOwner);
	bool bResult = GetWorld()->SweepSingleByChannel(hitResult, vStartPos, vEndPos, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5, FCollisionShape::MakeSphere(m_fSize), params);

	if (bResult && hitResult.GetActor())
	{
		vHitPos = hitResult.ImpactPoint;
		return hitResult.GetActor();
	}

	return nullptr;
}

