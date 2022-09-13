#include "KiLiGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"


UKiLiGameInstance::UKiLiGameInstance()
{
	m_currentGameState = GAME_STATE::START;
}

 