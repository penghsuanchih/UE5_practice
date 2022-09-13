#include "KiLiGameMode.h"
#include "GamePlayerController.h"
#include "KiLiGameInstance.h"
#include "ObjectItemManager.h"
#include "AgentsManager.h"
#include "Agent.h"
#include "HeroAgent.h"
#include "SkillManager.h"
#include "KiLiHUD.h"
#include "Projectile.h"
#include "EffectParticle.h"
#include "UObject/ConstructorHelpers.h"

AKiLiGameMode::AKiLiGameMode()
{
	m_bpHeroAgentClass[int(HERO_TYPE::ARCHER)] = Cast<UClass>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Hero/BP_Archer.BP_Archer_C")));
	m_bpHeroAgentClass[int(HERO_TYPE::KNIGHT)] = Cast<UClass>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Hero/BP_Knight.BP_Knight_C")));
	m_bpHeroAgentClass[int(HERO_TYPE::HUNTER)] = Cast<UClass>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Hero/BP_Hunter.BP_Hunter_C")));
	m_bpProjectileClass[int(PROJECTILE_EFFECT_TYPE::BULLET)] = Cast<UClass>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Item/Projectile/bp_bullet.bp_bullet_C")));
	m_bpProjectileClass[int(PROJECTILE_EFFECT_TYPE::BULLET_LARGE)] = Cast<UClass>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Item/Projectile/bp_bullet2.bp_bullet2_C")));
	m_bpProjectileClass[int(PROJECTILE_EFFECT_TYPE::ARROW)] = Cast<UClass>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Item/Projectile/bp_arrow.bp_arrow_C")));
	m_bpEffectParticleClass[int(VISIBLE_EFFECT_TYPE::BLOOD)] = Cast<UClass>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Item/EffectParticle/bp_blood.bp_blood_C")));
	m_bpEffectParticleClass[int(VISIBLE_EFFECT_TYPE::HEAL)] = Cast<UClass>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Item/EffectParticle/bp_heal.bp_heal_C")));
	
	m_bpDropObjectClass = Cast<UClass>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Item/bp_DropObject.bp_DropObject_C")));
		
	DefaultPawnClass = Cast<UClass>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Level/BP_AgentsManager.BP_AgentsManager_C")));
 	PlayerControllerClass = AGamePlayerController::StaticClass();
	HUDClass = AKiLiHUD::StaticClass();
	m_pSkillManager = nullptr;
	m_passedTime = 0.0;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AKiLiGameMode::BeginPlay()
{
	m_pObjectItemManager = GetWorld()->SpawnActor<AObjectItemManager>();
	m_pSkillManager = GetWorld()->SpawnActor<ASkillManager>();
	m_pSkillManager->SetGameMode(this);
}

void AKiLiGameMode::Tick(float deltaSeconds)
{
	UKiLiGameInstance* pGameInstance = Cast<UKiLiGameInstance>(GetWorld()->GetGameInstance());
	if (pGameInstance)
	{
		GAME_STATE currentGameState = pGameInstance->GetGameState();
		if (currentGameState == GAME_STATE::START)
			pGameInstance->SetGameState(GAME_STATE::INIT);
		else if (currentGameState == GAME_STATE::INIT)
		{
			if (m_passedTime < 0.5)
				m_passedTime += deltaSeconds;
			else
				pGameInstance->SetGameState(GAME_STATE::INIT_MAP);
		}
		else if (currentGameState == GAME_STATE::INIT_MAP_END)
		{
			if (m_passedTime < 2)
				m_passedTime += deltaSeconds;
			else
			{
				ShowAllAgents(true);
				pGameInstance->SetGameState(GAME_STATE::PLAY);
			}
		}
	}
}

AHeroAgent* AKiLiGameMode::SpawnHero(HERO_TYPE type, FVector vPos)
{
	return GetWorld()->SpawnActor<AHeroAgent>(m_bpHeroAgentClass[int(type)], vPos, FRotator::ZeroRotator);
}
 
AProjectile* AKiLiGameMode::SpawnProjectile(PROJECTILE_EFFECT_TYPE type, FVector vPos)
{
	return GetWorld()->SpawnActor<AProjectile>(m_bpProjectileClass[int(type)], vPos, FRotator::ZeroRotator);
}

AEffectParticle* AKiLiGameMode::SpawnEffectParticle(VISIBLE_EFFECT_TYPE type, FVector vPos)
{
	return GetWorld()->SpawnActor<AEffectParticle>(m_bpEffectParticleClass[int(type)], vPos, FRotator::ZeroRotator);
}

ADropObject* AKiLiGameMode::SpawnDropObject(FVector vPos, FName itemName)
{
	ADropObject* pDropObject = GetWorld()->SpawnActor<ADropObject>(m_bpDropObjectClass, vPos, FRotator::ZeroRotator);
	if (pDropObject)
	{
		ObjectItem* pObjectItem = m_pObjectItemManager->GetObjectItem(itemName);
		pDropObject->SetObjectItem(itemName, pObjectItem);
	}

	return pDropObject;
}

void AKiLiGameMode::ShowAllAgents(bool state)
{
	if (m_pAgentsManager)
		m_pAgentsManager->ShowAllAgents(state);
}