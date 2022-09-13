#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "KiLiHUD.generated.h"

enum TEXTURE_TYPE
{
	TEXTURE_CROSS = 0,
	TEXTURE_SELECT_FRAME,
	TEXTURE_HP_HERO_ICON,
	TEXTURE_HP_ENEMY_ICON,
	TEXTURE_HP_BG_ICON,
	TEXTURE_AGENT_ICON_1,
	TEXTURE_AGENT_ICON_2,
	TEXTURE_AGENT_ICON_3,
	TEXTURE_AGENT_ICON_FRAME,
	TEXTURE_AGENT_ICON_FRAME_SEL,
	TEXTURE_MINIMAP_FRAME,
	TEXTURE_COUNT
};

struct MiniMapData
{
	MiniMapData()
	{
		bInitedState = false;
	}

	bool bInitedState;
	int x, y;
	int size;
};

class AAgent;
class AAgentsManager;
class AKiLiGameMode;
class AMiniMapCapture;
UCLASS()
class AKiLiHUD : public AHUD
{
	GENERATED_BODY()
		AKiLiHUD();

public:
	virtual void DrawHUD() override;
 
private:
	void InitMiniMap(AKiLiGameMode* pGameMode);
	void DrawAllAgentsHP(AAgentsManager* pAgentMaster);
	void DrawHeroCards(AAgentsManager* pAgentsManager);
	void DrawGroupSelectedRect(AAgentsManager* pAgentsManager);
	void DrawIcon(UTexture2D* pTexture, FVector2D vPos, FVector2D vSize, float rot);
	void DrawHeadTopHpBar(UTexture2D* pHpTexture, AAgentsManager* pAgentMaster, AAgent* pAgent);
	void DrawAgentsHurtValue(AAgentsManager* pAgentMaster);
	void DrawAgentHurtValue(AAgentsManager* pAgentMaster, AAgent* pAgentCharacter);
	void DrawNumber(int x, int y, float size, long number);
	void DrawMiniMap(AKiLiGameMode* pGameMode, AAgentsManager* pAgentsManager);
	void DrawMiniMapAgentInfo(AAgent* pAgent, float pointSize, FLinearColor color);
	void UpdateCanvasUVTri(TArray<FCanvasUVTri>* pCanvasUVTri,  int x, int y, int width, int height, int startStep, int curStep, int maxStep);
	UTexture2D* m_pTextureList[TEXTURE_COUNT];
	UTexture2D* m_pNumberTexture[10];
	UTexture* m_pMiniMapTexture;
	float m_screenWidth, m_screenHeight;
	AMiniMapCapture* m_pMiniMapCapture;
	TArray<FCanvasUVTri> m_miniMapDrawTri;
	MiniMapData m_miniMapData;
 
};
