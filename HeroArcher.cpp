#include "HeroArcher.h"
#include "AgentsManager.h"
#include "SkillManager.h"

AHeroArcher::AHeroArcher()
{
	const ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> bpAnimObj(TEXT("AnimBlueprint'/Game/Hero/ABP_Archer.ABP_Archer_C'"));
	m_pAnimClass = bpAnimObj.Object;
	m_data.name = "Archer";
}
 
void AHeroArcher::BeginPlay()
{
	Super::BeginPlay();

	FName defaultName[] = {"A_TopBody_A", "A_BotBody_A", "A_hand", "A_head" , "A_hair", "A_shoe" };
	EQUIP_ITEM_TYPE type[] = {EQUIP_ITEM_TYPE::TOP_BODY, EQUIP_ITEM_TYPE::BOT_BODY, EQUIP_ITEM_TYPE::HAND, EQUIP_ITEM_TYPE::HEAD, EQUIP_ITEM_TYPE::HAIR, EQUIP_ITEM_TYPE::SHOE};
	for (int i = 0; i < int(EQUIP_ITEM_TYPE::COUNT); i++)
		SetDefaultEquipItem(type[i], defaultName[i]);

	SetDefaultWeaponItem("bow");
}

void AHeroArcher::StartAttack()
{
	SKILL_TYPE skillType[3] = { SKILL_TYPE::ARROW_SHOT, SKILL_TYPE::ARROW_SHOT_MULTI, SKILL_TYPE::ARROW_SHOT_RAIN };
	m_pSkillManager->UseSkill(this, skillType[m_data.nextSkillIndex]);
}