#pragma once
#include "CoreMinimal.h"
 
UENUM(BlueprintType)
enum class GAME_STATE : uint8
{
	NONE = 0,
	TITLE,
	ROLE_SWITCH, 
	START,
	INIT,
	INIT_MAP,
	INIT_MAP_END,
	PLAY
};
