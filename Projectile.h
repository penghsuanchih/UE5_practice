#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "ParticleEffectType.h"
#include "Projectile.generated.h"

 
class AAgent;
class AKiLiGameMode;
UCLASS()
class AProjectile : public AStaticMeshActor
{
	GENERATED_BODY()

public:	
	AProjectile();
	virtual void Tick(float DeltaTime) override;
	virtual bool Fire(AAgent* pAgent, FVector vPos, FVector vDir, long damage, float speed, float delayTime, float size);
	FVector GetDirection() {return m_vDir;}
	AActor* HitDetect(FVector vStartPos, FVector vEndPos, FVector& vHitPos);
 
protected:
	void UpdateFire(AKiLiGameMode* pGameMode, FVector vStartPos, FVector vEndPos);
	float m_fDeltaSeconds;
	FVector m_vDir;
	AAgent* m_pOwner;
	long m_damage;
	float m_fDelayTime;
	float m_fSpeed;
	float m_fSize;
	bool m_bStopMove;
	bool m_bDisappear;
};
