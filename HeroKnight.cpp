#include "HeroKnight.h"
#include "AgentsManager.h"
#include "SkillManager.h"

AHeroKnight::AHeroKnight()
{
 	const ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> bpAnimObj(TEXT("AnimBlueprint'/Game/Hero/ABP_Knight.ABP_Knight_C'"));
	m_pAnimClass = bpAnimObj.Object;
	m_data.name = "Knight";
}
 
void AHeroKnight::BeginPlay()
{
	Super::BeginPlay();

	SetDefaultEquipItem(EQUIP_ITEM_TYPE::TOP_BODY, "B_TopBody_A");
}

void AHeroKnight::StartAttack()
{
	SKILL_TYPE skillType[3] = { SKILL_TYPE::ARROW_SHOT, SKILL_TYPE::ARROW_SHOT_MULTI, SKILL_TYPE::ARROW_SHOT_RAIN };
	m_pSkillManager->UseSkill(this, skillType[m_data.nextSkillIndex]);
}