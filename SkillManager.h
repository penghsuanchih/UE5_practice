#pragma once

#include "CoreMinimal.h"
#include "ParticleEffectType.h"
#include "SkillData.h"
#include "GameFramework/Actor.h"
#include "SkillManager.generated.h"

class AAgent;
class AKiLiGameMode;
UCLASS()
class ASkillManager : public AActor
{
	GENERATED_BODY()
	ASkillManager();

public:
	void SetGameMode(AKiLiGameMode* pGameMode) {m_pGameMode = pGameMode;}
	bool UseSkill(AAgent* pAgent, SKILL_TYPE type);
	void OnUpdate(AAgent* pAgent);
	void OnAttackBegin(AAgent* pAgent, SKILL_TYPE type);
	bool OnAttackEnd(AAgent* pAgent, SKILL_TYPE type);

private:
	//Gun
	void GunShot_Single(AAgent* pAgent);
	void GunShot_Large(AAgent* pAgent);
	void GunShot_Multi(AAgent* pAgent);
//Arrow
	void ArrowShot_Single(AAgent* pAgent);
	void ArrowShot_Multi(AAgent* pAgent);
	void ArrowShot_Rain(AAgent* pAgent);
//Sword
	void SwordAttack(AAgent* pAgent);

//Physics
	void UpdatePhysicsAttack(AAgent* pAgent);

	bool CheckCanUseSkill(AAgent* pAgent);
	void Shot(AAgent* pAgent, PROJECTILE_EFFECT_TYPE type, int shotCount, int angleStep, int multiCount, int damage, float speed, float delayTime, float size);
	void  FireProjectile(AAgent* pAgent, PROJECTILE_EFFECT_TYPE type, FVector vPos, FVector vTargetPos, int damage, float speed, float delayTime, float size);
	AKiLiGameMode* m_pGameMode;
};
