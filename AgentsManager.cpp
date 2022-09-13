#include "AgentsManager.h"
#include "KiLiGameMode.h"
#include "KiLiGameInstance.h"
#include "ObjectItemManager.h"
#include "GamePlayerController.h"
#include "Agent.h"
#include "HeroAgent.h"
#include "EnemyAgent.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"


AAgentsManager::AAgentsManager()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = DEFAULT_CAMERA_ZOOM_DISTANCE;
	CameraBoom->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	m_pMoveTargetActorClass = Cast<UClass>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Level/bp_MoveTarget.bp_MoveTarget_C")));
	//m_pSpawingCursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	//static ConstructorHelpers::FObjectFinder<UMaterial> decalMaterialAsset(TEXT("Material'/Game/Cursor/M_Cursor_Decal.M_Cursor_Decal'"));
	//if (decalMaterialAsset.Succeeded())
	//{
	//	m_pSpawingCursorToWorld->SetDecalMaterial(decalMaterialAsset.Object);
	//	m_pSpawingCursorToWorld->DecalSize = FVector(16.0f, 48.0f, 48.0f);
	//	m_pSpawingCursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	//}

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	m_pMoveTargetActor = nullptr;
	m_pGameMode = nullptr;
	m_pPlayerController = nullptr;
	m_pObjectItemManager = nullptr;
}

void AAgentsManager::BeginPlay()
{
	Super::BeginPlay();
 
	UKiLiGameInstance* pGameInstance = Cast<UKiLiGameInstance>(GetWorld()->GetGameInstance());
	GAME_STATE currentGameState = pGameInstance->GetGameState();
	m_pGameMode = Cast<AKiLiGameMode>(GetWorld()->GetAuthGameMode());
	m_pPlayerController = Cast<AGamePlayerController>(GetController());
	m_pObjectItemManager = m_pGameMode->GetObjectItemManager();
	m_pGameMode->SetAgentsManager(this);

	if (currentGameState == GAME_STATE::ROLE_SWITCH)
	{
		InitForHeroSwitch();
		ShowAllAgents(true);
	}
	else
	{
	    InitCamera(); 
		InitInventory();
		AddHeroCard();
		OnEnableHeroCard(0, GetActorLocation());
		m_inventory.AddObjectItemToEquip("bow");
		m_inventory.AddObjectItemToBag("A_TopBody_B", 1);
		m_inventory.AddObjectItemToBag("A_BotBody_B", 1);
		m_inventory.AddObjectItemToBag("sword", 1);
		m_inventory.AddObjectItemToBag("potion", 1);
		m_inventory.AddObjectItemToToolBar(0, "skill_1");
		m_inventory.AddObjectItemToToolBar(1, "skill_2");
		m_inventory.AddObjectItemToToolBar(2, "skill_3");
	}
}

void AAgentsManager::Tick(float deltaSeconds)
{
    Super::Tick(deltaSeconds);
	
	UKiLiGameInstance* pGameInstance = Cast<UKiLiGameInstance>(GetWorld()->GetGameInstance());
	if (pGameInstance)
	{
		GAME_STATE currentGameState = pGameInstance->GetGameState();

		UpdateAgentCharactersList();
		UpdateCamera(currentGameState, deltaSeconds);
		if (currentGameState == GAME_STATE::ROLE_SWITCH)
			UpdateSwitchHeroes(deltaSeconds, false);
		else if (currentGameState == GAME_STATE::PLAY)
		{
			UpdateInputData(deltaSeconds);
			UpdateAllHeroCardsData();
			UpdateSpawingCursorToWorld();
			UpdateAllEnemyRenderCustomDepthState();
		}

	}
}
 
void AAgentsManager::UpdateAgentCharactersList()
{
	TArray<AActor*> actorsArray;
	m_agentGroupData.Clear();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHeroAgent::StaticClass(), actorsArray);
	for (AActor* pActor : actorsArray)
	{
		AHeroAgent* pHero = Cast<AHeroAgent>(pActor);
		if (pHero->GetHP())
			m_agentGroupData.ourAgentsList.Add(pHero);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyAgent::StaticClass(), actorsArray);
	for (AActor* pActor : actorsArray)
	{
		AEnemyAgent* pEnemy = Cast<AEnemyAgent>(pActor);
		if (pEnemy->GetHP())
			m_agentGroupData.enemyAgentsList.Add(pEnemy);
	}
}

void AAgentsManager::OnNotifyAgentCharactersSelectedState(TArray<AAgent*> selectedAgentsList)
{
	m_agentGroupData.selectedAgentsList.Empty();
	for (AHeroAgent* pHero : m_agentGroupData.selectedAgentsList)
		m_agentGroupData.selectedAgentsList.Add(pHero);

	UpdateAgentCharactersSelectedState();
}
 
void AAgentsManager::UpdateAgentCharactersSelectedState()
{
	for (AHeroAgent* pHero : m_agentGroupData.ourAgentsList)
		pHero->SetSelectedState(false);

	m_heroCardData.ClearSelect();
	for (AHeroAgent* pHero : m_agentGroupData.selectedAgentsList)
	{
		m_heroCardData.Select(pHero);
		pHero->SetSelectedState(true);
	}
}
 
void AAgentsManager::OnNitifyAgentDeath(AAgent* pAgent)
{
	UpdateAgentCharactersList();

	for (AHeroAgent* pHero : m_agentGroupData.ourAgentsList)
		pHero->OnNotifiedAgentDied(pAgent);

	for (AEnemyAgent* pEnemy : m_agentGroupData.enemyAgentsList)
		pEnemy->OnNotifiedAgentDied(pAgent);

	if (pAgent->IsA(AHeroAgent::StaticClass()))
		m_heroCardData.Disable(Cast<AHeroAgent>(pAgent));
}

bool AAgentsManager::CheckPressedOnAgent(AActor* pActor)
{
	if (pActor && pActor->IsA(AAgent::StaticClass()))
		return true;

	return false;
}

bool AAgentsManager::CheckPressedOnEnemy(AActor* pActor)
{
	if (CheckPressedOnAgent(pActor))
	{
		AAgent* pAgent = Cast<AAgent>(pActor);
		return (pAgent->GetGroupType() == AGENT_GROUP_TYPE::ENEMY)? true : false;
	}

	return false;
}

void AAgentsManager::SingleSelectHero(AHeroAgent* pHero)
{
	if (pHero)
	{
		m_agentGroupData.selectedAgentsList.Empty();
		m_agentGroupData.selectedAgentsList.Add(pHero);
 		UpdateAgentCharactersSelectedState();
		MoveCameraTo(pHero);
	}
}

FVector2D AAgentsManager::GetScreenPos(FVector vPos)
{
	FVector2D vTargetPos = FVector2D(0, 0);
	m_pPlayerController->ProjectWorldLocationToScreen(vPos, vTargetPos);
	return vTargetPos;
}

USkeletalMesh* AAgentsManager::GetWeaponMesh(FName name)
{
	ObjectItem* pObjectItem = m_pObjectItemManager->GetObjectItem(name);
	return pObjectItem->skeletalMesh;
}

void AAgentsManager::OnSwitchSkill(int index)
{
	for (AAgent* pAgent : m_agentGroupData.selectedAgentsList)
		pAgent->OnSwitchSkill(index);
}

void AAgentsManager::UpdateAllEnemyRenderCustomDepthState()
{
	float patch = 45.0 * 3.14 / 180;
	float height = CameraBoom->TargetArmLength * sin(patch);
	float distance = CameraBoom->TargetArmLength * cos(patch);
	FVector vCameraPos = GetActorLocation() + FVector(-distance, 0, height);

	for (AHeroAgent* pHero : m_agentGroupData.ourAgentsList)
		pHero->UpdateRenderCustomDepthState(vCameraPos);

	for (AAgent* pEnemy : m_agentGroupData.enemyAgentsList)
		pEnemy->UpdateRenderCustomDepthState(vCameraPos);
}
 
void AAgentsManager::ShowMoveTarget(bool bState, FVector vPos)
{
	if (m_pMoveTargetActor)
	{
		m_pMoveTargetActor->Destroy();
		m_pMoveTargetActor = nullptr;
	}

	if (bState)
		m_pMoveTargetActor = GetWorld()->SpawnActor<AActor>(m_pMoveTargetActorClass, vPos, FRotator::ZeroRotator);
}

void AAgentsManager::ShowAllAgents(bool state)
{
	for (AAgent* pEnemy : m_agentGroupData.enemyAgentsList)
		pEnemy->SetActorHiddenInGame(!state);

	for (AHeroAgent* pHero : m_agentGroupData.ourAgentsList)
		pHero->SetActorHiddenInGame(!state);
}

ObjectItem* AAgentsManager::GetObjectItem(FName name)
{
	return (m_pObjectItemManager)? m_pObjectItemManager->GetObjectItem(name) : nullptr;
}

USkeletalMesh* AAgentsManager::GetHeroEquipMesh(EQUIP_ITEM_TYPE type)
{
	for (AHeroAgent* pHero : m_agentGroupData.selectedAgentsList)
		return pHero->GetEquipMesh(type);

	return nullptr;
}

USkeletalMesh* AAgentsManager::GetHeroWeaponMesh()
{
	for (AHeroAgent* pHero : m_agentGroupData.selectedAgentsList)
		return pHero->GetWeponMesh();

	return nullptr;
}

void AAgentsManager::OnUpdateEquipItem()
{
	for (AHeroAgent* pHero : m_agentGroupData.selectedAgentsList)
	{
		pHero->SetEquipItem(EQUIP_ITEM_TYPE::TOP_BODY, m_inventory.GetEquipItemInfo(0)->id);
		pHero->SetEquipItem(EQUIP_ITEM_TYPE::BOT_BODY, m_inventory.GetEquipItemInfo(1)->id);
		pHero->SetWeapon(m_inventory.GetWeaponItemInfo()->id);
	}
}

FName AAgentsManager::GetWeaponSocketName()
{
	for (AHeroAgent* pHero : m_agentGroupData.selectedAgentsList)
		return pHero->GetWeaponSocketName();

	return "none";
}

int AAgentsManager::RotateTo(int startAngle, int targetAngle, int moveRotAngle)
{
	int rotYawStart = startAngle;
	int rotYawEnd = targetAngle;
	int defAngle = rotYawEnd - rotYawStart;
	if (defAngle < -180) { rotYawStart -= 360; }
	else if (defAngle > 180) { rotYawEnd -= 360; }

	defAngle = targetAngle - rotYawStart;
	if (defAngle > 0)
	{
		rotYawStart += moveRotAngle;
		if (rotYawStart >= rotYawEnd)
			return targetAngle;
	}
	else if (defAngle < 0)
	{
		rotYawStart -= moveRotAngle;
		if (rotYawStart <= rotYawEnd)
			return targetAngle;
	}
	else
		return  targetAngle;

	return rotYawStart;
}

FName AAgentsManager::GetCurrentSelectedHeroName()
{
	for (AHeroAgent* pHero : m_agentGroupData.selectedAgentsList)
		return pHero->GetData()->name;

	return "   ";
}

int32 AAgentsManager::GetCurrentSelectedSkillIndex()
{
	if (m_agentGroupData.selectedAgentsList.Num())
		return m_agentGroupData.selectedAgentsList[0]->GetData()->nextSkillIndex;

	return -1;
}