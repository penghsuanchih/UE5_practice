#include "AgentData.h"
#include "Setting.h"

AgentData::AgentData()
{
	animActionType = AGENT_ANIM_ACTION_TYPE::IDLE;
	nextAnimActionType = animActionType;
	bSelectedState = false;
	targetHP = 100;
	curHP = 100;
	groupType = AGENT_GROUP_TYPE::PLAYER;
	curSkillType = SKILL_TYPE::NONE;
	nextSkillIndex = 0;
	curAttackIndex = -1;
	bComboAttack = false;
	attackLockTime = 0.0;
	name = "   ";
}

void AgentData::AddShowHurtData(long _value)
{
	ShowHurtData data(_value);
	showHurtDataList.Add(data);
}

void AgentData::Update(float deltaSeconds)
{
	if (curHP > targetHP)
	{
		curHP -= deltaSeconds * UPDATE_BLOOD_SPEED;
		if (curHP < targetHP)
			curHP = targetHP;
	}
	else if (curHP < targetHP)
	{
		curHP += deltaSeconds * UPDATE_BLOOD_SPEED;
		if (curHP > targetHP)
			curHP = targetHP;
	}

	for (int i = 0; i < showHurtDataList.Num(); i++)
	{
		if (showHurtDataList[i].passedTime >= SHOW_HURT_DISAPPEAR_TIME)
			showHurtDataList.RemoveAt(i);
		else
		{
			float sizeScaleList[10] = {0.3, 0.5, 0.6, 0.7, 1.2, 1.2, 1.2, 0.6, 0.5, 0.4};
			float upScaleList[10] = {0.2, 0.2, 0.2, 0.3, 0.1, 0.1, 0.1, 0.2, 0.2, 0.1};
			int index = (showHurtDataList[i].passedTime * 10) / (SHOW_HURT_DISAPPEAR_TIME + 0.1);
			showHurtDataList[i].scale = sizeScaleList[index];
			showHurtDataList[i].height += SHOW_HURT_UP_SPEED * upScaleList[index];
			showHurtDataList[i].passedTime += deltaSeconds;
		}
	}
}