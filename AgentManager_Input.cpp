#include "AgentsManager.h"
#include "KiLiGameMode.h"
#include "GamePlayerController.h"
#include "HeroAgent.h"
#include "EnemyAgent.h"
#include "DropObject.h"

void AAgentsManager::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	InputComponent->BindAction("LMouseButton", IE_Pressed, this, &AAgentsManager::OnInputPressed);
	InputComponent->BindAction("LMouseButton", IE_Released, this, &AAgentsManager::OnInputReleased);
	InputComponent->BindAction("CameraZoomIn", IE_Pressed, this, &AAgentsManager::OnCameraZoomIn);
	InputComponent->BindAction("CameraZoomOut", IE_Released, this, &AAgentsManager::OnCameraZoomOut);
	InputComponent->BindAction("Key_1", IE_Pressed, this, &AAgentsManager::OnKeyPressed_1);
	InputComponent->BindAction("Key_2", IE_Pressed, this, &AAgentsManager::OnKeyPressed_2);
	InputComponent->BindAction("Key_3", IE_Pressed, this, &AAgentsManager::OnKeyPressed_3);
	InputComponent->BindAction("Key_Up", IE_Pressed, this, &AAgentsManager::OnKeyPressed_Up);
	InputComponent->BindAction("Key_Down", IE_Pressed, this, &AAgentsManager::OnKeyPressed_Down);
	InputComponent->BindAction("Key_Left", IE_Pressed, this, &AAgentsManager::OnKeyPressed_Left);
	InputComponent->BindAction("Key_Right", IE_Pressed, this, &AAgentsManager::OnKeyPressed_Right);
	InputComponent->BindAction("Key_Up", IE_Released, this, &AAgentsManager::OnKeyReleased_Direction);
	InputComponent->BindAction("Key_Down", IE_Released, this, &AAgentsManager::OnKeyReleased_Direction);
	InputComponent->BindAction("Key_Left", IE_Released, this, &AAgentsManager::OnKeyReleased_Direction);
	InputComponent->BindAction("Key_Right", IE_Released, this, &AAgentsManager::OnKeyReleased_Direction);
}

void AAgentsManager::OnInputPressed()
{
	m_heroCardData.index = GetSelectedHeroCard(m_inputData.vCurCursorPos.X, m_inputData.vCurCursorPos.Y);
	m_inputData.vPressedCursorPos = m_inputData.vCurCursorPos;
	m_inputData.type = INPUT_PRESSED;
	m_inputData.pressedTime = 0.0;
}

void AAgentsManager::OnInputReleased()
{
	if (m_inputData.type == INPUT_PRESSED)//點擊事件
	{
		if (m_heroCardData.index >= 0)
			SingleSelectHero(m_heroCardData.GetHero(m_heroCardData.index));
		else if(m_inputData.pTargetActor)
		{
			if (CheckPressedOnAgent(m_inputData.pTargetActor)) //點擊到角色?
			{
				if (m_inputData.pTargetActor->IsA(AEnemyAgent::StaticClass()))//Enemy
					OnPressedOnEnemy(Cast<AEnemyAgent>(m_inputData.pTargetActor));
				else
					OnPressedOnHero(Cast<AHeroAgent>(m_inputData.pTargetActor));
			}
			else if (m_inputData.pTargetActor->IsA(ADropObject::StaticClass()))//DropObject
			{
				if (AddDropObjectToInventory(Cast<ADropObject>(m_inputData.pTargetActor)))
					m_inputData.pTargetActor = nullptr;
			}
			else
			{
				m_cameraData.mode = CAMERA_FOLLOW;
				ShowMoveTarget(true, m_inputData.vTargetPos);
				for (AHeroAgent* pHero : m_agentGroupData.selectedAgentsList)
					pHero->MoveTo(m_inputData.vTargetPos);
			}
		}
	}

	m_inputData.type = INPUT_NONE;
	if (m_heroCardData.index >= 0) //拖曳事件
	{
		if (GetSelectedHeroCard(m_inputData.vCurCursorPos.X, m_inputData.vCurCursorPos.Y) == -1)//有離開卡片
			OnEnableHeroCard(m_heroCardData.index, m_inputData.vTargetPos);//激活卡片
	}

	m_heroCardData.index = -1;
	m_inputData.pressedTime = 0.0;
}
 
void AAgentsManager::OnKeyPressed_1()
{
	OnSwitchSkill(0);
}

void AAgentsManager::OnKeyPressed_2()
{
	OnSwitchSkill(1);
}

void AAgentsManager::OnKeyPressed_3()
{
	OnSwitchSkill(2);
}

void AAgentsManager::OnKeyPressed_Up()
{
//	m_cameraData.moveDirectionType = CAMERA_MOVE_DIRECTION_UP;
//	m_cameraData.mode = CAMERA_FREE;
}

void AAgentsManager::OnKeyPressed_Down()
{
//	m_cameraData.moveDirectionType = CAMERA_MOVE_DIRECTION_DOWN;
//	m_cameraData.mode = CAMERA_FREE;
}

void AAgentsManager::OnKeyPressed_Left()
{
//	m_cameraData.moveDirectionType = CAMERA_MOVE_DIRECTION_LEFT;
//	m_cameraData.mode = CAMERA_FREE;
}

void AAgentsManager::OnKeyPressed_Right()
{
//	m_cameraData.moveDirectionType = CAMERA_MOVE_DIRECTION_RIGHT;
//	m_cameraData.mode = CAMERA_FREE;
}

void AAgentsManager::OnKeyReleased_Direction()
{
	m_cameraData.moveDirectionType = CAMERA_MOVE_DIRECTION_NONE;
}

void AAgentsManager::UpdateInputData(float deltaSeconds)
{
 	FHitResult traceHitResult;
	m_pPlayerController->GetMousePosition(m_inputData.vCurCursorPos.X, m_inputData.vCurCursorPos.Y);
	if (m_pPlayerController->GetHitResultUnderCursor(ECC_Pawn, true, traceHitResult))
	{
		m_inputData.pTargetActor = traceHitResult.GetActor();
		m_inputData.vTargetPos = traceHitResult.ImpactPoint;
		m_inputData.vTargetNormal = traceHitResult.ImpactNormal;
	}
	else
		m_inputData.pTargetActor = nullptr;

	//if (m_inputData.type == INPUT_PRESSED)
	//{
	//	if (m_heroCardData.index == -1)
	//	{
	//		m_inputData.pressedTime += deltaSeconds;
	//		if (CheckCursorRangeSelectState())
	//			m_inputData.type = INPUT_RANGE_SELECT;
	//	}
	//} 
}

bool AAgentsManager::CheckCursorRangeSelectState()
{
	if (m_inputData.pressedTime >= INPUT_CLICKED_WAIT_TIME || m_inputData.GetCursorMoveDistance() >= INPUT_CLICKED_MOVE_DISTANCE)
		return true;

	return false;
}

void AAgentsManager::OnPressedOnEnemy(AEnemyAgent* pPressedEnemy)
{
	for (AHeroAgent* pHero : m_agentGroupData.selectedAgentsList)
		pHero->AttackTo(pPressedEnemy);
}

void AAgentsManager::OnPressedOnHero(AHeroAgent* pPressedHero)
{
	SingleSelectHero(pPressedHero);
}