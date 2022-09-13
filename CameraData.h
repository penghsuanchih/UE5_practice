#pragma once
#include "CoreMinimal.h"

enum CAMERA_MOVE_DIRECTION_TYPE
{
	CAMERA_MOVE_DIRECTION_UP = 0,
	CAMERA_MOVE_DIRECTION_RIGHT,
	CAMERA_MOVE_DIRECTION_DOWN,
	CAMERA_MOVE_DIRECTION_LEFT,
	CAMERA_MOVE_DIRECTION_NONE
};

enum CAMERA_MODE
{
	CAMERA_FOLLOW = 0,
	CAMERA_FREE
};

struct CameraData
{
	CameraData()
	{
		moveDirectionType = CAMERA_MOVE_DIRECTION_NONE;
		movePassedTime = 0.0;
		mode = CAMERA_FOLLOW;
	}

	CAMERA_MODE mode;
	CAMERA_MOVE_DIRECTION_TYPE moveDirectionType;
	FVector vTargetCameraPos;
	float movePassedTime;
};