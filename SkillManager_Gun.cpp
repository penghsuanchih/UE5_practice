#include "SkillManager.h"
#include "Agent.h"
#include "Setting.h"

void ASkillManager::GunShot_Single(AAgent* pAgent)
{
	Shot(pAgent, PROJECTILE_EFFECT_TYPE::BULLET, 1, 0, 1, 20, BULLET_MOVE_SPEED, 0, 15);
}

void ASkillManager::GunShot_Large(AAgent* pAgent)
{
	Shot(pAgent, PROJECTILE_EFFECT_TYPE::BULLET_LARGE, 1, 0, 1, 50, BULLET_MOVE_SPEED, 0.3, 50);
}

void ASkillManager::GunShot_Multi(AAgent* pAgent)
{
	Shot(pAgent, PROJECTILE_EFFECT_TYPE::BULLET, 5, 10, 1, 20, BULLET_MOVE_SPEED, 0.3, 15);
}

