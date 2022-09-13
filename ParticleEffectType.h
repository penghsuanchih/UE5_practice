#pragma once

UENUM(BlueprintType)
enum class  PROJECTILE_EFFECT_TYPE : uint8
{
	BULLET = 0,
	BULLET_LARGE,
	ARROW,
	COUNT,
	NONE
};


UENUM(BlueprintType)
enum class  VISIBLE_EFFECT_TYPE : uint8
{
	BLOOD = 0,
    HEAL,
	COUNT,
	NONE
};