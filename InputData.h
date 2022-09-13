#pragma once
#include "CoreMinimal.h"


enum INPUT_TYPE
{
	INPUT_NONE = 0,
	INPUT_PRESSED,
	INPUT_CAMERA,
	INPUT_RANGE_SELECT
};

struct UserInputData
{
	UserInputData()
	{
		pressedTime = 0.0;
		type = INPUT_NONE;
		pTargetActor = nullptr;
	}

	int GetCursorMoveDistance()
	{
		return (vCurCursorPos - vPressedCursorPos).Size();
	}

	INPUT_TYPE type;
	FVector2D vCurCursorPos;
	FVector2D vPressedCursorPos;
	FVector vTargetPos, vTargetNormal;
	AActor* pTargetActor;
	float pressedTime;
};
 