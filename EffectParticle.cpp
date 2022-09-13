#include "EffectParticle.h"
 
AEffectParticle::AEffectParticle()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
	m_fDeltaSeconds = 0.0;
}
 
void AEffectParticle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
 
	m_fDeltaSeconds += DeltaTime;
	if (m_fDeltaSeconds >= 2.0)
		Destroy();
}
