#pragma once
#include "CoreMinimal.h"
#include "SkillData.h"

class AHeroAgent;
class AEnemyAgent;
struct AgentGroupData
{
	void Clear()
	{
		ourAgentsList.Empty();
		enemyAgentsList.Empty();
	}

	TArray<AHeroAgent*> selectedAgentsList;
	TArray<AHeroAgent*> ourAgentsList;
	TArray<AEnemyAgent*> enemyAgentsList;
};

UENUM(BlueprintType)
enum class AGENT_ANIM_ACTION_TYPE : uint8
{
	IDLE = 0,
	MOVE,
	ATTACK,
	ATTACK_READY,
	HURT,
	ROAM,
	CHASE,
	DEATH,
	DESTROY,
	DISAPEAR,
	NONE
};
 
UENUM(BlueprintType)
enum class HERO_ROLE_TYPE : uint8
{
	GUNNER = 0,
	ARCHER,
	SWORDSMAN,
	COUNT
};

UENUM(BlueprintType)
enum class AGENT_GROUP_TYPE : uint8
{
	PLAYER = 0,
	ENEMY
};

struct ShowHurtData
{
	ShowHurtData(long _value)
	{
		value = _value;
		passedTime = 0.0;
		scale = 1.0;
		height = 0;
	}

	long value;
	float passedTime;
	float scale;
	int height;
};

struct AgentData
{
	AgentData();
	void AddShowHurtData(long _value);
	void Update(float deltaSeconds);

	AGENT_GROUP_TYPE groupType;
	AGENT_ANIM_ACTION_TYPE animActionType, nextAnimActionType;
	SKILL_TYPE curSkillType;
	int curAttackIndex;
	int nextSkillIndex;
	HERO_ROLE_TYPE roleType;
	bool bSelectedState;
	int curHP, targetHP;
	bool bComboAttack;
	TArray<ShowHurtData> showHurtDataList;
	float attackLockTime;
	FVector vTargetPos;
	FName name;
};

