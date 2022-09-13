#include "AgentsManager.h"
#include "HeroAgent.h"
#include "KiLiGameMode.h"
#include "Setting.h"

void AAgentsManager::InitForHeroSwitch()
{
	m_roleSwitchData.vCenterPos = GetActorLocation();
	for (int i = 0; i < int(HERO_TYPE::COUNT); i++)
	{
		AHeroAgent* pHero = m_pGameMode->SpawnHero(HERO_TYPE(i), m_roleSwitchData.vCenterPos);
		if (pHero)
		{
			pHero->SetActorRotation(FRotator(0, 180, 0));
			pHero->SetSelectedState(false);
			m_agentGroupData.ourAgentsList.Add(pHero);
		}
	}

	SwitchHero(0);
}

void AAgentsManager::SwitchHero(int rotAngle)
{
	int distance = 150;
	int heroesCount = m_agentGroupData.ourAgentsList.Num();
	if (heroesCount)
	{
		int stepAngle = 360 / heroesCount;
		for (int i = 0; i < heroesCount; i++)
		{
			float angle = (-90 + rotAngle + (360 - i * stepAngle)) * 3.14 / 180;
			FVector vPos = m_roleSwitchData.vCenterPos + FVector(sin(angle) * distance, cos(angle) * distance, 0);
			m_agentGroupData.ourAgentsList[i]->SetActorLocation(vPos);
		}
	}
}

void AAgentsManager::SwitchLastHero()
{
	if (m_roleSwitchData.curSwitchingAngle == m_roleSwitchData.targetSwitchAngle && m_agentGroupData.ourAgentsList.Num())
	{
		int stepAngle = 360 / m_agentGroupData.ourAgentsList.Num();
		m_roleSwitchData.targetSwitchAngle -= stepAngle;
		if (m_roleSwitchData.targetSwitchAngle < 0)
			m_roleSwitchData.targetSwitchAngle += 360;
	}
}

void AAgentsManager::SwitchNextHero()
{
 	if (m_roleSwitchData.curSwitchingAngle == m_roleSwitchData.targetSwitchAngle && m_agentGroupData.ourAgentsList.Num())
 	{
    	int stepAngle = 360 / m_agentGroupData.ourAgentsList.Num();
		m_roleSwitchData.targetSwitchAngle += stepAngle;
 		if (m_roleSwitchData.targetSwitchAngle >= 360)
			m_roleSwitchData.targetSwitchAngle -= 360;
 	}
}

void AAgentsManager::SetCurrentSwitchedHeroIndex(int index)
{
	m_roleSwitchData.targetSwitchAngle = 360 / m_agentGroupData.ourAgentsList.Num() * index;
	m_roleSwitchData.curSwitchingAngle = m_roleSwitchData.targetSwitchAngle;
 	UpdateSwitchHeroes(0, true);
}

void AAgentsManager::UpdateSwitchHeroes(float deltaSeconds, bool bForce)
{
 	if (!m_agentGroupData.ourAgentsList.Num()) {}
 	else if (bForce)
 		SwitchHero(m_roleSwitchData.curSwitchingAngle);
 	else if (m_roleSwitchData.curSwitchingAngle != m_roleSwitchData.targetSwitchAngle)
   {
		m_roleSwitchData.curSwitchingAngle = RotateTo(m_roleSwitchData.curSwitchingAngle, m_roleSwitchData.targetSwitchAngle, SWITCH_HERO_ROT_SPEED * deltaSeconds);
 		SwitchHero(m_roleSwitchData.curSwitchingAngle);
		m_agentGroupData.selectedAgentsList.Empty();
 	}
 	else
 	{
 		int stepAngle = 360 / m_agentGroupData.ourAgentsList.Num();
		m_roleSwitchData.curSwitchedHeroIndex = m_roleSwitchData.curSwitchingAngle / stepAngle;
		m_agentGroupData.selectedAgentsList.Empty();
		m_agentGroupData.selectedAgentsList.Add(m_agentGroupData.ourAgentsList[m_roleSwitchData.curSwitchedHeroIndex]);
 	}
}