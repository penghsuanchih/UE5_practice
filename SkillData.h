#pragma once
#include "CoreMinimal.h"
 
UENUM(BlueprintType)
enum class SKILL_TYPE : uint8
{
	NONE = 0,
	ARROW_SHOT,
	ARROW_SHOT_MULTI,
	ARROW_SHOT_RAIN,
	GUN_SHOT,
	GUN_SHOT_LARGE,
	GUN_SHOT_MULTI,
	SWORD_ATTACK,
	ZOMBIE_ATTACK
};