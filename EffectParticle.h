#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "EffectParticle.generated.h"

  
UCLASS()
class AEffectParticle : public AStaticMeshActor
{
	GENERATED_BODY()

public:	
	AEffectParticle();
	virtual void Tick(float DeltaTime) override;
	float m_fDeltaSeconds;
};
