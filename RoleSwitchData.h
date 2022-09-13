#pragma once
#include "CoreMinimal.h"

struct RoleSwitchData
{
	RoleSwitchData()
	{
		curSwitchedHeroIndex = 0;
		curSwitchingAngle = 0;
		targetSwitchAngle = 0;
	}

	FVector vCenterPos;
	int curSwitchingAngle, targetSwitchAngle;
	int curSwitchedHeroIndex;
};

