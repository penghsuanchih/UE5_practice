#include "AgentsManager.h"
#include "GameFramework/SpringArmComponent.h"

void AAgentsManager::InitCamera()
{
	m_cameraData.vTargetCameraPos = GetActorLocation();
}

void AAgentsManager::OnCameraZoomIn()
{
	CameraBoom->TargetArmLength -= CAMERA_ZOOM_SPEED;
	if (CameraBoom->TargetArmLength < MIN_CAMERA_ZOOM_DISTANCE)
		CameraBoom->TargetArmLength = MIN_CAMERA_ZOOM_DISTANCE;
}

void AAgentsManager::OnCameraZoomOut()
{
	CameraBoom->TargetArmLength += CAMERA_ZOOM_SPEED;
	if (CameraBoom->TargetArmLength > MAX_CAMERA_ZOOM_DISTANCE)
		CameraBoom->TargetArmLength = MAX_CAMERA_ZOOM_DISTANCE;
}

void AAgentsManager::MoveCameraTo(CAMERA_MOVE_DIRECTION_TYPE type, float deltaSeconds)
{
	FVector vDir[4] = { GetActorForwardVector(), GetActorRightVector(), GetActorForwardVector() * -1, GetActorRightVector() * -1};
	m_cameraData.vTargetCameraPos +=  vDir[type] * DEFAULT_CAMERA_MOVE_DISTANCE * deltaSeconds;
}

void AAgentsManager::MoveCameraTo(AAgent* pAgent)
{
	m_cameraData.vTargetCameraPos = pAgent->GetActorLocation();
	m_cameraData.movePassedTime = 0.0;
}

void AAgentsManager::UpdateCamera(GAME_STATE state, float deltaSeconds)
{
	FVector vTargetPos;
	if (state == GAME_STATE::TITLE)
	{
		CameraBoom->TargetArmLength = DEFAULT_LOADOUT_CAMERA_ISTANCE;
		CameraBoom->SetRelativeLocation(FVector(0, 0, 100));
		CameraBoom->SetRelativeRotation(FRotator(0, 210, 0));
		TopDownCameraComponent->FieldOfView = 90;
		vTargetPos = m_cameraData.vTargetCameraPos;
	}
	else if (state == GAME_STATE::ROLE_SWITCH)
	{
		CameraBoom->TargetArmLength = DEFAULT_LOADOUT_CAMERA_ISTANCE;
		CameraBoom->SetRelativeLocation(FVector(-60, 0, 90));
		CameraBoom->SetRelativeRotation(FRotator(0, 0, 0));
		TopDownCameraComponent->FieldOfView = 45;
		vTargetPos = m_cameraData.vTargetCameraPos;
	}
	else if (m_cameraData.mode == CAMERA_FOLLOW)
	{
		if (m_agentGroupData.selectedAgentsList.Num())
		{
			FVector vDir = m_agentGroupData.selectedAgentsList[0]->GetActorLocation() - GetActorLocation();
			int distance = vDir.Size2D();
			if (distance < DEFAULT_CAMERA_MOVE_DISTANCE)
				vTargetPos = m_agentGroupData.selectedAgentsList[0]->GetActorLocation();
			else
			{
				vDir.Normalize();
				distance = DEFAULT_CAMERA_MOVE_SPEED * deltaSeconds;
				vTargetPos = GetActorLocation() + FVector(vDir.X * distance, vDir.Y * distance, 0);
			}
		}
	}

	SetActorLocation(vTargetPos);
}