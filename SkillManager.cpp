#include "SkillManager.h"
#include "KiLiGameMode.h"
#include "Agent.h"
#include "Projectile.h"

ASkillManager::ASkillManager()
: m_pGameMode(nullptr)
{
}

bool ASkillManager::CheckCanUseSkill(AAgent* pAgent)
{
	AAgent* pTargetEnemy = pAgent->GetTargetEnemy();
	return (pTargetEnemy)? true : false;
}

bool ASkillManager::UseSkill(AAgent* pAgent, SKILL_TYPE type)
{
	if (CheckCanUseSkill(pAgent))
	{
		if (pAgent->GetAnimActionState() == AGENT_ANIM_ACTION_TYPE::ATTACK)
		{
			if (type == SKILL_TYPE::SWORD_ATTACK)
				pAgent->SetComboAttackSate(true);
		}
		else
		{
			pAgent->SetCurrentSkillType(type);
			pAgent->SetAnimActionType(AGENT_ANIM_ACTION_TYPE::ATTACK);
		}

		return true;
	}

	return false;
}

void ASkillManager::OnAttackBegin(AAgent* pAgent, SKILL_TYPE type)
{
	if (type == SKILL_TYPE::ARROW_SHOT)
		ArrowShot_Single(pAgent);
	else if (type == SKILL_TYPE::ARROW_SHOT_MULTI)
		ArrowShot_Multi(pAgent);
	else if (type == SKILL_TYPE::ARROW_SHOT_RAIN)
		ArrowShot_Rain(pAgent);
	else if (type == SKILL_TYPE::GUN_SHOT)
		GunShot_Single(pAgent);
	else if (type == SKILL_TYPE::GUN_SHOT_LARGE)
		GunShot_Large(pAgent);
	else if (type == SKILL_TYPE::GUN_SHOT_MULTI)
		GunShot_Multi(pAgent);
	else if (type == SKILL_TYPE::SWORD_ATTACK)
		SwordAttack(pAgent);
	else
		return;

	pAgent->GetData()->curSkillType = type;
}

bool ASkillManager::OnAttackEnd(AAgent* pAgent, SKILL_TYPE type)
{
	//if (pAgent->GetAnimActionState() == AGENT_ANIM_ACTION_TYPE::ATTACK)
	//{
		//if (type == SKILL_TYPE::SWORD_ATTACK && pAgent->GetComboAttackSate())
		//{
		//	SwordAttack(pAgent);
		//	return true;
		//}

		//pAgent->SetComboAttackSate(false);
	//}

	return false;
}

void ASkillManager::OnUpdate(AAgent* pAgent)
{
	if (pAgent->GetCurrentSkillType() == SKILL_TYPE::SWORD_ATTACK ||
		pAgent->GetCurrentSkillType() == SKILL_TYPE::ZOMBIE_ATTACK)
		UpdatePhysicsAttack(pAgent);
}

void ASkillManager::Shot(AAgent* pAgent, PROJECTILE_EFFECT_TYPE type, int shotCount, int angleStep, int multiCount, int damage, float speed, float delayTime, float size)
{
	AAgent* pTargetEnemy = pAgent->GetTargetEnemy();
	FVector vStartPos = pAgent->GetWeaponSocketPosition();
	FVector vTargetPos = pTargetEnemy->GetActorLocation() + FVector(0, 0, 60);
	FVector vTargetDir = (vTargetPos - vStartPos).GetSafeNormal();
	int startAngle = (shotCount == 1)? 0 : ((shotCount - 1) * angleStep * -0.5);
	for (int i = 0; i < multiCount; i++)
	{
		for (int j = 0; j < shotCount; j++)
		{
			int angle = startAngle + j * angleStep;
			FVector vCurDir = vTargetDir.RotateAngleAxis(angle, FVector(0, 0, 1));
			vTargetPos = vStartPos + vCurDir * 1000;
			FireProjectile(pAgent, type, vStartPos, vTargetPos, damage, speed, delayTime * i , size);
		}
	}
}

void ASkillManager::FireProjectile(AAgent* pAgent, PROJECTILE_EFFECT_TYPE type, FVector vPos, FVector vTargetPos, int damage, float speed, float delayTime, float size)
{
	FVector vDir = (vTargetPos - vPos).GetSafeNormal();
	AProjectile* pBullet = m_pGameMode->SpawnProjectile(type, vPos);
	if (pBullet)
		pBullet->Fire(pAgent, vPos, vDir, damage, speed, delayTime, size);
}

void ASkillManager::UpdatePhysicsAttack(AAgent* pAgent)
{
	FVector vStartPos = pAgent->GetWeaponAttackSocketPosition("start");
	FVector vEndPos = pAgent->GetWeaponAttackSocketPosition("end");
	TArray<FHitResult> hitActors;
	FCollisionShape collisionShape;
	FCollisionQueryParams params;
	params.AddIgnoredActor(pAgent);
	collisionShape.ShapeType = ECollisionShape::Sphere;
	collisionShape.SetSphere(30);
	if (GetWorld()->SweepMultiByChannel(hitActors, vStartPos, vEndPos, FQuat(), ECC_Pawn, collisionShape, params))
	{
		for (auto hitActorsItr = hitActors.CreateIterator(); hitActorsItr; hitActorsItr++)
		{
			AActor* pHitActor = hitActorsItr->GetActor();
			if (pHitActor && pHitActor->IsA(AAgent::StaticClass()))
				(Cast<AAgent>(pHitActor))->TakeAttackDamage(0.5, pAgent, false, hitActorsItr->ImpactPoint);
		}
	}
}
