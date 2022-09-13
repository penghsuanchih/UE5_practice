#include "SkillManager.h"
#include "Agent.h"
#include "Setting.h"

void ASkillManager::SwordAttack(AAgent* pAgentCharacter)
{
	int attackIndex = 0;
	if (pAgentCharacter->GetComboAttackSate())
       attackIndex =  pAgentCharacter->GetCurrentAttackIndex() + 1;

	pAgentCharacter->SetCurrentAttackIndex((attackIndex >= 4)? 0 : attackIndex);
	pAgentCharacter->SetComboAttackSate(false);
}
