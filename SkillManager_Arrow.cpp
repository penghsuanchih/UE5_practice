#include "SkillManager.h"
#include "Agent.h"
#include "Setting.h"

void ASkillManager::ArrowShot_Single(AAgent* pAgent)
{
	Shot(pAgent, PROJECTILE_EFFECT_TYPE::ARROW, 1, 0, 1, 20, ARROW_MOVE_SPEED, 0, 15);
}

void ASkillManager::ArrowShot_Multi(AAgent* pAgent)
{
 	AAgent* pTargetEnemy = pAgent->GetTargetEnemy();
 	FVector vTargetPos = pTargetEnemy->GetActorLocation();
	FVector vDir = (vTargetPos - GetActorLocation()).GetSafeNormal();
	FVector vCurDir = pAgent->GetActorRightVector();
 	FVector vPos[3] =
 	{ 
 		pAgent->GetWeaponSocketPosition(),
 		pAgent->GetWeaponSocketPosition() + vCurDir * 100,
 		pAgent->GetWeaponSocketPosition() - vCurDir * 100
 	};

 	FVector vTargetDir = vTargetPos - pAgent->GetActorLocation();
 	for (int i = 0; i < 3; i++)
 		FireProjectile(pAgent, PROJECTILE_EFFECT_TYPE::ARROW, vPos[i], vPos[i] + vTargetDir, 20, ARROW_MOVE_SPEED, 0, 15);
}

void ASkillManager::ArrowShot_Rain(AAgent* pAgent)
{
	int step = 120;
	long height = 500;
	float speed = ARROW_MOVE_SPEED * 1.8;
	float delayTime = 0.0;
	AAgent* pTargetEnemy = pAgent->GetTargetEnemy();
	FVector vTargetPos = pTargetEnemy->GetActorLocation();
	FVector vTargetDir = vTargetPos - pAgent->GetActorLocation();
	FVector vCurDir = pAgent->GetActorRightVector();
	int distance2 = 30 ;
	int distance = vTargetDir.Size();
	vTargetDir.Normalize();

	for (int x = -2; x < 2; x++)
	{
		for (int y = -2; y < 2; y++)
		{
			int xPos = FMath::RandRange(int(step * -0.3), int(step * 0.3));
			int yPos = FMath::RandRange(int(step * -0.3), int(step * 0.3));
			int zPos = height + FMath::RandRange(-step, step);
			float speed2 = FMath::RandRange(int(-step * 0.2), int(step * 0.2));
			FVector vStartPos = vTargetPos + FVector(x * step + xPos, y * step + yPos, zPos);
			FVector vEndPos = vStartPos + FVector(0, 0, -height);
			vStartPos -= vTargetDir * distance;
			FireProjectile(pAgent, PROJECTILE_EFFECT_TYPE::ARROW, vStartPos, vEndPos, 20, speed + speed2, delayTime, 15);
		}

		delayTime += 0.5;
	}
}