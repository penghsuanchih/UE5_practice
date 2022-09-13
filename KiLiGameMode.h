#pragma once

#include "CoreMinimal.h"
#include "AgentData.h"
#include "HeroData.h"
#include "DropObject.h"
#include "ParticleEffectType.h"
#include "SkillManager.h"
#include "GameFramework/GameModeBase.h"
#include "KiLiGameMode.generated.h"


class AAgent;
class AAgentsManager;
class AObjectItemManager;
class AProjectile;
class AHeroAgent;
class AEffectParticle;
UCLASS(minimalapi)
class AKiLiGameMode : public AGameModeBase
{
	GENERATED_BODY()
	AKiLiGameMode();
	virtual void BeginPlay() override;
	virtual void Tick(float deltaSeconds) override;

public:
	AHeroAgent* SpawnHero(HERO_TYPE type, FVector vPos);
	AProjectile* SpawnProjectile(PROJECTILE_EFFECT_TYPE type, FVector vPos);
	AEffectParticle* SpawnEffectParticle(VISIBLE_EFFECT_TYPE type, FVector vPos);
	ADropObject* SpawnDropObject(FVector vPos, FName itemName);   
	AObjectItemManager* GetObjectItemManager() {return m_pObjectItemManager;}
	AAgentsManager* GetAgentsManager() {return m_pAgentsManager;}
	ASkillManager* GetSkillManager() {return m_pSkillManager;}
	void SetAgentsManager(AAgentsManager* pAgentsManager) {m_pAgentsManager = pAgentsManager;}
	int GetScreenWidth() {return m_screenWidth;}
	int GetScreenHeight() {return m_screenHeight;}
	void SetScreenSize(int width, int height) {m_screenWidth = width; m_screenHeight = height;}
	void ShowAllAgents(bool state);
 
private:
	AObjectItemManager* m_pObjectItemManager;
	AAgentsManager* m_pAgentsManager;	
	TSubclassOf<AHeroAgent> m_bpHeroAgentClass[int(HERO_TYPE::COUNT)];
	TSubclassOf<AAgent> m_bpAgentClass[int(HERO_ROLE_TYPE::COUNT)];
	TSubclassOf<AProjectile> m_bpProjectileClass[int(PROJECTILE_EFFECT_TYPE::COUNT)];
	TSubclassOf<AEffectParticle> m_bpEffectParticleClass[int(VISIBLE_EFFECT_TYPE::COUNT)];
	TSubclassOf<ADropObject> m_bpDropObjectClass;

	int m_screenWidth, m_screenHeight;
 	ASkillManager* m_pSkillManager;
	float m_passedTime;
};



