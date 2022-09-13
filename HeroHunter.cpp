#include "HeroHunter.h"
#include "AgentsManager.h"
#include "SkillManager.h"

AHeroHunter::AHeroHunter()
{
 	const ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> bpAnimObj(TEXT("AnimBlueprint'/Game/Hero/ABP_Hunter.ABP_Hunter_C'"));
	m_pAnimClass = bpAnimObj.Object;
	m_data.name = "Hunter";
}
 
void AHeroHunter::BeginPlay()
{
	Super::BeginPlay();

	SetDefaultEquipItem(EQUIP_ITEM_TYPE::TOP_BODY, "C_TopBody_A");
}

void AHeroHunter::StartAttack()
{
	SKILL_TYPE skillType[3] = { SKILL_TYPE::ARROW_SHOT, SKILL_TYPE::ARROW_SHOT_MULTI, SKILL_TYPE::ARROW_SHOT_RAIN };
	m_pSkillManager->UseSkill(this, skillType[m_data.nextSkillIndex]);
}