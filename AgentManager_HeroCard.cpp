#include "AgentsManager.h"
#include "KiLiGameMode.h"
#include "AgentPlayerController.h"
#include "Components/DecalComponent.h"
#include "KiLiHUD.h"

void AAgentsManager::AddHeroCard()
{
	HeroCard card;
	m_heroCardData.heroCardsList.Add(card);
}

void AAgentsManager::UpdateAllHeroCardsData()
{
	int screenWidth = m_pGameMode->GetScreenWidth();
	int screenHeight = m_pGameMode->GetScreenHeight();
	int cardWidth = screenWidth * 0.06;
	int cardHeight = cardWidth * 1.0;
	int cardStep = cardWidth * 0.1;
	int xPos = cardStep;
	int yPos = screenHeight - (cardHeight + cardStep);
	for (int i = 0; i < m_heroCardData.heroCardsList.Num(); i++)
		m_heroCardData.heroCardsList[i].SetRect(xPos + i * (cardWidth + cardStep), yPos, cardWidth, cardHeight);
}

int AAgentsManager::GetSelectedHeroCard(int x, int y)
{
	for (int i = 0; i < m_heroCardData.heroCardsList.Num(); i++)
	{
		if (m_heroCardData.GetState(i) == HERO_CARD_STATE::DISABLED) {}
		else
		{
			if (x > m_heroCardData.heroCardsList[i].x && x < (m_heroCardData.heroCardsList[i].x + m_heroCardData.heroCardsList[i].width) &&
				y > m_heroCardData.heroCardsList[i].y && y < (m_heroCardData.heroCardsList[i].y + m_heroCardData.heroCardsList[i].height))
				return i;
		}
	}

	return -1;
}

void AAgentsManager::UpdateSpawingCursorToWorld()
{
//	bool bHideState = true;
//	if (m_heroCardData.index != -1 && m_heroCardData.GetState(m_heroCardData.index) == HERO_CARD_STATE::UNENABLE)
//	{
//		m_pSpawingCursorToWorld->SetWorldLocation(m_inputData.vTargetPos);
//		m_pSpawingCursorToWorld->SetWorldRotation(m_inputData.vTargetNormal.Rotation());
//		bHideState = false;
//	}
 
//	m_pSpawingCursorToWorld->SetHiddenInGame(bHideState);
}

void AAgentsManager::OnEnableHeroCard(int index, FVector vPos)
{
	if (m_heroCardData.GetState(index) == HERO_CARD_STATE::UNENABLE)
	{
		AHeroAgent* pHero = m_pGameMode->SpawnHero(HERO_TYPE::ARCHER, vPos);
		m_heroCardData.Enable(index, pHero);
		SingleSelectHero(pHero);
	}
}

void AAgentsManager::OnSelectHeroCard(int index)
{
	if (m_heroCardData.Select(index))
		SingleSelectHero(m_heroCardData.GetHero(index));
}