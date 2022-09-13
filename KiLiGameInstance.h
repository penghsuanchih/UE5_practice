#pragma once

#include "CoreMinimal.h"
#include "GameData.h"
#include "Engine/GameInstance.h"
#include "Containers/Ticker.h"
#include "KiLiGameInstance.generated.h"

 
UCLASS()
class UKiLiGameInstance : public UGameInstance
{
	GENERATED_BODY()
	UKiLiGameInstance();
 
public:
	GAME_STATE GetGameState() { return m_currentGameState; }

	UFUNCTION(BlueprintCallable, Category = Properties)
		void SetGameState(GAME_STATE state) { m_currentGameState = state; }

private:
	GAME_STATE m_currentGameState;

};
