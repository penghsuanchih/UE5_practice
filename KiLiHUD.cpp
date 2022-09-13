#include "KiLiHUD.h"
#include "KiLiGameMode.h"
#include "KiLiGameInstance.h"
#include "AgentsManager.h"
#include "HeroAgent.h"
#include "EnemyAgent.h"
#include "ObjectItemManager.h"
#include "MiniMapCapture.h"
#include "Engine/Canvas.h"
#include "Kismet/GameplayStatics.h"
#include "Setting.h"

AKiLiHUD::AKiLiHUD()
: m_screenWidth(0), m_screenHeight(0), m_pMiniMapTexture(nullptr)
{
	m_pTextureList[TEXTURE_CROSS] = LoadTextureFromPath("Texture2D'/Game/UI/texture/cross.cross'");
	m_pTextureList[TEXTURE_SELECT_FRAME] = LoadTextureFromPath("Texture2D'/Game/UI/texture/select-frame.select-frame'");
	m_pTextureList[TEXTURE_HP_HERO_ICON] = LoadTextureFromPath(TEXT("Texture2D'/Game/UI/texture/color_3.color_3'"));
	m_pTextureList[TEXTURE_HP_ENEMY_ICON] = LoadTextureFromPath(TEXT("Texture2D'/Game/UI/texture/color_2.color_2'"));
	m_pTextureList[TEXTURE_HP_BG_ICON] = LoadTextureFromPath(TEXT("Texture2D'/Game/UI/texture/color_bg.color_bg'"));
	m_pTextureList[TEXTURE_AGENT_ICON_1] = LoadTextureFromPath(TEXT("Texture2D'/Game/UI/texture/AgentIcon1.AgentIcon1'"));
	m_pTextureList[TEXTURE_AGENT_ICON_2] = LoadTextureFromPath(TEXT("Texture2D'/Game/UI/texture/AgentIcon2.AgentIcon2'"));
	m_pTextureList[TEXTURE_AGENT_ICON_3] = LoadTextureFromPath(TEXT("Texture2D'/Game/UI/texture/AgentIcon3.AgentIcon3'"));
	m_pTextureList[TEXTURE_AGENT_ICON_FRAME] = LoadTextureFromPath(TEXT("Texture2D'/Game/UI/texture/AgentFrame.AgentFrame'"));
	m_pTextureList[TEXTURE_AGENT_ICON_FRAME_SEL] = LoadTextureFromPath(TEXT("Texture2D'/Game/UI/texture/AgentFrameSel.AgentFrameSel'"));
	m_pTextureList[TEXTURE_MINIMAP_FRAME] = LoadTextureFromPath(TEXT("Texture2D'/Game/UI/texture/minimap-frame.minimap-frame'"));

	for (int i = 0; i <10; i++)
		m_pNumberTexture[i] = LoadTextureFromPath(FString::Printf(TEXT("Texture2D'/Game/UI/texture/w%d.w%d'"), i, i));
}

void AKiLiHUD::DrawHUD()
{
	Super::DrawHUD();
	 
	AKiLiGameMode* pGameMode = Cast<AKiLiGameMode>(GetWorld()->GetAuthGameMode());
	UKiLiGameInstance* pGameInstance = Cast<UKiLiGameInstance>(GetWorld()->GetGameInstance());
	if (pGameInstance)
	{
		GAME_STATE currentGameState = pGameInstance->GetGameState();
		if (currentGameState == GAME_STATE::INIT_MAP)
		{
 			InitMiniMap(pGameMode);
			pGameInstance->SetGameState(GAME_STATE::INIT_MAP_END);
		}
		else if (currentGameState == GAME_STATE::PLAY)
		{
			AAgentsManager* pAgentsManager = pGameMode->GetAgentsManager();
			pGameMode->SetScreenSize(Canvas->SizeX, Canvas->SizeY);
			DrawAllAgentsHP(pAgentsManager);
			DrawGroupSelectedRect(pAgentsManager);
			DrawHeroCards(pAgentsManager);
			DrawAgentsHurtValue(pAgentsManager);
 			DrawMiniMap(pGameMode, pAgentsManager);
		}
	}
}

void AKiLiHUD::InitMiniMap(AKiLiGameMode* pGameMode)
{
	TArray<AActor*> pActorsArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMiniMapCapture::StaticClass(), pActorsArray);
	for (AActor* pActor : pActorsArray)
		m_pMiniMapCapture = Cast<AMiniMapCapture>(pActor);
	
	m_miniMapData.size = Canvas->SizeX * DEFAULT_MINIMAP_MINIMAP_SIZE_RATE;
	m_miniMapData.x = m_miniMapData.size * 0.05;
	m_miniMapData.y = m_miniMapData.size * 0.05;
	m_pMiniMapCapture->UpdateWorldBounds();
	UpdateCanvasUVTri(&m_miniMapDrawTri, m_miniMapData.x, m_miniMapData.y, m_miniMapData.size, m_miniMapData.size, 0, MAX_MINIMAP_DRAW_STEP_COUNT, MAX_MINIMAP_DRAW_STEP_COUNT);
	m_pMiniMapTexture = Cast<UTexture>(m_pMiniMapCapture->GetCaptureComponent2D()->TextureTarget);
}

void  AKiLiHUD::DrawAllAgentsHP(AAgentsManager* pAgentMaster)
{
	AgentGroupData* pAgentGroupData = pAgentMaster->GetAgentGroupData();
	for (AEnemyAgent* pEnemy : pAgentGroupData->enemyAgentsList)
		DrawHeadTopHpBar(m_pTextureList[TEXTURE_HP_ENEMY_ICON], pAgentMaster, pEnemy);

	for (AHeroAgent* pHero : pAgentGroupData->ourAgentsList)
		DrawHeadTopHpBar(m_pTextureList[TEXTURE_HP_HERO_ICON], pAgentMaster, pHero);
}

void AKiLiHUD::DrawHeroCards(AAgentsManager* pAgentsManager)
{
	HeroCardData* pCardData = pAgentsManager->GetHeroCardData();
	int cardCount = pCardData->heroCardsList.Num();
	for (int i = 0; i < cardCount; i++)
	{
		HeroCard* pCard = &pCardData->heroCardsList[i];
		DrawIcon(m_pTextureList[TEXTURE_AGENT_ICON_1 + i], FVector2D(pCard->x, pCard->y), FVector2D(pCard->width, pCard->height), 0);
		DrawIcon(m_pTextureList[TEXTURE_AGENT_ICON_FRAME], FVector2D(pCard->x, pCard->y), FVector2D(pCard->width, pCard->height), 0);
		if (pCard->state == HERO_CARD_STATE::DISABLED)
			DrawIcon(m_pTextureList[TEXTURE_CROSS], FVector2D(pCard->x, pCard->y), FVector2D(pCard->width, pCard->height), 0);
		else if (pCard->state == HERO_CARD_STATE::SELECTED)
		{
			int frameSize = pCard->width * 0.1;
			DrawIcon(m_pTextureList[TEXTURE_AGENT_ICON_FRAME_SEL], FVector2D(pCard->x - frameSize, pCard->y - frameSize), FVector2D(pCard->width + frameSize * 2, pCard->height + frameSize * 2), 0);
		}
	}
}

void AKiLiHUD::DrawGroupSelectedRect(AAgentsManager* pAgentsManager)
{
	UserInputData* pInputData = pAgentsManager->GetUserInputData();
	if (pInputData->type == INPUT_RANGE_SELECT)
	{
		AgentGroupData* pAgentGroupData = pAgentsManager->GetAgentGroupData();
		FVector2D vStartPos = pInputData->vPressedCursorPos;
		FVector2D vEndPos = pInputData->vCurCursorPos;
		TArray<AAgent*> selectedAgentsList;
		DrawRect(FLinearColor(0.0, 0.0, 1.0, 0.1), vStartPos.X, vStartPos.Y, vEndPos.X - vStartPos.X, vEndPos.Y - vStartPos.Y);
		GetActorsInSelectionRectangle<AAgent>(vStartPos, vEndPos, selectedAgentsList, false, false);
		pAgentsManager->OnNotifyAgentCharactersSelectedState(selectedAgentsList);
	}
}

void AKiLiHUD::DrawIcon(UTexture2D* pTexture, FVector2D vPos, FVector2D vSize, float rot)
{
	if (pTexture)
	{
		FCanvasTileItem tileItem(vPos, pTexture->Resource, FLinearColor::White);
		tileItem.Rotation = FRotator(0, rot, 0);
		tileItem.BlendMode = SE_BLEND_Translucent;
		tileItem.Size = vSize;
		Canvas->DrawItem(tileItem);
	}
}

void AKiLiHUD::DrawHeadTopHpBar(UTexture2D* pHpTexture, AAgentsManager* pAgentMaster, AAgent* pAgent)
{
	AgentData* pData = pAgent->GetData();
	if (pData->curHP)
	{
		float m_fScaleY = 1.0;
		int length = (1 + pAgent->maxHP / HUD_DEFAULT_MAX_HP) * HUD_DEFAULT_HP * HP_BAR_SCALE * m_fScaleY;//test
		int height = 8 * m_fScaleY;
		float scale = (pData->curHP * 1.0) / pAgent->maxHP;
		FVector2D vPos = pAgentMaster->GetScreenPos(pAgent->GetActorLocation() + FVector(0, 0, 120));
		vPos -= FVector2D(length * 0.5, 0);
		DrawIcon(m_pTextureList[TEXTURE_HP_BG_ICON], vPos, FVector2D(length, height), 0);
		DrawIcon(pHpTexture, vPos, FVector2D(length * scale, height), 0);
	}
}

void AKiLiHUD::DrawAgentsHurtValue(AAgentsManager* pAgentMaster)
{
	AgentGroupData* pAgentGroupData = pAgentMaster->GetAgentGroupData();
	for (AEnemyAgent* pEnemy : pAgentGroupData->enemyAgentsList)
		DrawAgentHurtValue(pAgentMaster, pEnemy);
}

void AKiLiHUD::DrawAgentHurtValue(AAgentsManager* pAgentMaster, AAgent* pAgent)
{
	AgentData* pData = pAgent->GetData();
	FVector2D vStartPos = pAgentMaster->GetScreenPos(pAgent->GetActorLocation() + FVector(0, 0, 30));
	for (int i = 0; i < pData->showHurtDataList.Num(); i++)
	{
		FVector2D vPos = vStartPos - FVector2D(0, pData->showHurtDataList[i].height);
		DrawNumber(vPos.X, vPos.Y, SHOW_HURT_SCALE * pData->showHurtDataList[i].scale, pData->showHurtDataList[i].value);
	}
}

void AKiLiHUD::DrawNumber(int x, int y, float size, long number)
{
	int numberIconSizeX[11] = {52, 27, 48, 47, 53, 48, 48, 48, 48, 48, 19};
	int numberIconSizeY = 104;
	int numberSizeStepX = 19;
	TArray<int> numberList;
	long curNumber = number;
	for (;;)
	{
		numberList.Add(curNumber % 10);
		curNumber /= 10;
		if (curNumber == 0)
			break;
	}

	int numberTotalWidth = (numberList.Num() - 1) * numberSizeStepX * size;
	for (int i = 0; i < numberList.Num(); i++)
		numberTotalWidth += numberIconSizeX[numberList[i]] * size;

	int xStart = x - numberTotalWidth * 0.5;
	for (int i = numberList.Num() - 1; i >= 0; i--)
	{
		int index = numberList[i];
		DrawIcon(m_pNumberTexture[index], FVector2D(xStart, y - numberIconSizeY * 0.5 * size), FVector2D(numberIconSizeX[index] * size, numberIconSizeY * size), 0);
		xStart += (numberIconSizeX[index] + numberSizeStepX) * size;
	}
}
 
void AKiLiHUD::DrawMiniMap(AKiLiGameMode* pGameMode, AAgentsManager* pAgentsManager)
{
	if (m_pMiniMapTexture)
	{
		FVector vCenterPos = FVector::ZeroVector;
		int pointSize = 5;
		float frameSize = m_miniMapData.size * 0.1;
		Canvas->K2_DrawTriangle(m_pMiniMapTexture, m_miniMapDrawTri);

		AgentGroupData* pAgentGroupData = pAgentsManager->GetAgentGroupData();
		for (AAgent* pAgent : pAgentGroupData->enemyAgentsList)
			DrawMiniMapAgentInfo(pAgent, pointSize, FLinearColor::Red);

		for (AAgent* pHero : pAgentGroupData->ourAgentsList)
			DrawMiniMapAgentInfo(pHero, pointSize, FLinearColor::Blue);

		DrawIcon(m_pTextureList[TEXTURE_MINIMAP_FRAME], FVector2D(m_miniMapData.x, m_miniMapData.y),
			 FVector2D(m_miniMapData.size, m_miniMapData.size), 0);
	}
}

void AKiLiHUD::DrawMiniMapAgentInfo(AAgent* pAgent, float pointSize, FLinearColor color)
{
	if (pAgent->GetHP() > 0)
	{
		FVector vPos = pAgent->GetActorLocation();
		int mapSizeX = m_pMiniMapCapture->MapPosRightDown.X - m_pMiniMapCapture->MapPosLeftTop.X;
		int mapSizeY = m_pMiniMapCapture->MapPosRightDown.Y - m_pMiniMapCapture->MapPosLeftTop.Y;
		int x = m_miniMapData.x + ((vPos.Y - m_pMiniMapCapture->MapPosLeftTop.Y) / mapSizeY) * m_miniMapData.size;
		int y = m_miniMapData.y + ((vPos.X - m_pMiniMapCapture->MapPosLeftTop.X) / mapSizeX) * m_miniMapData.size;
		DrawRect(color, x - pointSize * 0.5, y - pointSize * 0.5, pointSize, pointSize);
	}
}

void AKiLiHUD::UpdateCanvasUVTri(TArray<FCanvasUVTri>* pCanvasUVTri, int x, int y, int width, int height, int startStep, int curStep, int maxStep)
{
	FCanvasUVTri triangle;
	float uList[MAX_MINIMAP_DRAW_STEP_COUNT + 1];
	float vList[MAX_MINIMAP_DRAW_STEP_COUNT + 1];
	int xList[MAX_MINIMAP_DRAW_STEP_COUNT + 1];
	int yList[MAX_MINIMAP_DRAW_STEP_COUNT + 1];
	int length = FMath::Max(width, height);
	int xStart, yStart, xCenter, yCenter, startAngle;
	int insideStartIndex = maxStep;
	int triCount = 1;
	int maxDrawStep[3] = { curStep - 1, curStep, curStep - 1 };
	triangle.V0_Color = FLinearColor(1.0, 1.0, 1.0, 1.0);
	triangle.V1_Color = FLinearColor(1.0, 1.0, 1.0, 1.0);
	triangle.V2_Color = FLinearColor(1.0, 1.0, 1.0, 1.0);
	startAngle = -90;
	xStart = x;
	yStart = y;

	xCenter = xStart + width * 0.5;
	yCenter = yStart + height * 0.5;
	xList[MAX_MINIMAP_DRAW_STEP_COUNT] = xCenter;
	yList[MAX_MINIMAP_DRAW_STEP_COUNT] = yCenter;
	uList[MAX_MINIMAP_DRAW_STEP_COUNT] = 0.5;
	vList[MAX_MINIMAP_DRAW_STEP_COUNT] = 0.5;
	for (int i = startStep; i < curStep; i++)
	{
		float angle = (startAngle + i * (360 / maxStep)) * 3.14 / 180;
		double insideWidth[2] = { width * 0.5, width * 0.05 };
		double insideHeight[2] = { height * 0.5, height * 0.05 };
		int index[2] = { i, insideStartIndex + i };
		for (int j = 0; j < triCount; j++)
		{
			xList[index[j]] = xCenter + ((FMath::Abs(length * FMath::Cos(angle)) < insideWidth[j]) ? FMath::Abs(length * FMath::Cos(angle)) : insideWidth[j]) * ((FMath::Cos(angle) > 0) ? 1 : -1);
			yList[index[j]] = yCenter + ((FMath::Abs(length * FMath::Sin(angle)) < insideHeight[j]) ? FMath::Abs(length * FMath::Sin(angle)) : insideHeight[j]) * ((FMath::Sin(angle) > 0) ? 1 : -1);
			uList[index[j]] = (xList[index[j]] - xStart) / (width * 1.0);
			vList[index[j]] = (yList[index[j]] - yStart) / (height * 1.0);
		}
	}

	pCanvasUVTri->Empty();
	for (int i = 0; i < MAX_MINIMAP_DRAW_STEP_COUNT; i++)
	{
		int index01 = i;
		int index02 = ((i + 1) >= maxStep) ? 0 : (i + 1);
		int insideIndex = MAX_MINIMAP_DRAW_STEP_COUNT;
		int indexList[2][3] =
		{
			{insideIndex, index01, index02},
			{index02, insideStartIndex + index02, insideStartIndex + index01}
		};

		for (int j = 0; j < triCount; j++)
		{
			triangle.V0_Pos.X = xList[indexList[j][0]];
			triangle.V0_Pos.Y = yList[indexList[j][0]];
			triangle.V0_UV.X = uList[indexList[j][0]];
			triangle.V0_UV.Y = vList[indexList[j][0]];
			triangle.V1_Pos.X = xList[indexList[j][1]];
			triangle.V1_Pos.Y = yList[indexList[j][1]];
			triangle.V1_UV.X = uList[indexList[j][1]];
			triangle.V1_UV.Y = vList[indexList[j][1]];
			triangle.V2_Pos.X = xList[indexList[j][2]];
			triangle.V2_Pos.Y = yList[indexList[j][2]];
			triangle.V2_UV.X = uList[indexList[j][2]];
			triangle.V2_UV.Y = vList[indexList[j][2]];
			pCanvasUVTri->Add(triangle);
		}
	}
}
