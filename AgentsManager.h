#pragma once

#include "CoreMinimal.h"
#include "Inventory.h"
#include "HeroCard.h"
#include "AgentData.h"
#include "InputData.h"
#include "CameraData.h"
#include "RoleSwitchData.h"
#include "GameData.h"
#include "Setting.h"
#include "GameFramework/Character.h"
#include "AgentsManager.generated.h"

class AKiLiGameMode;
class AGamePlayerController;
class AObjectItemManager;
class AAgent;
class ADropObject;
UCLASS(Blueprintable)
class AAgentsManager : public ACharacter
{
	GENERATED_BODY()

public:
	AAgentsManager();
	virtual void BeginPlay() override;
	virtual void Tick(float deltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	HeroCardData* GetHeroCardData() {return &m_heroCardData;}
	UserInputData* GetUserInputData() {return &m_inputData;}
	AgentGroupData* GetAgentGroupData() {return &m_agentGroupData;}
	void OnNotifyAgentCharactersSelectedState(TArray<AAgent*> selectedAgentsList);
	void OnNitifyAgentDeath(AAgent* pAgent);
	FVector2D GetScreenPos(FVector vPos);
	USkeletalMesh* GetWeaponMesh(FName name);
	void UpdateAllEnemyRenderCustomDepthState();
	void ShowAllAgents(bool state);
	ObjectItem* GetObjectItem(FName name);
	void OnUpdateEquipItem();
	bool UseObjectItem(FName name);

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	UFUNCTION(BlueprintCallable, Category = Properties)
		void InitForHeroSwitch();

	UFUNCTION(BlueprintCallable, Category = Properties)
		void SwitchNextHero();

	UFUNCTION(BlueprintCallable, Category = Properties)
		void SwitchLastHero();

	UFUNCTION(BlueprintCallable, Category = Properties)
		UTexture2D* GetInventoryItemImage(int index);

	UFUNCTION(BlueprintCallable, Category = Properties)
		UTexture2D* GetInventoryItemBgImage(int index);

	UFUNCTION(BlueprintCallable, Category = Properties)
		int GetInventoryItemCount(int index);

	UFUNCTION(BlueprintCallable, Category = Properties)
		void OnInventoryItemClickedNotify(int index);

	UFUNCTION(BlueprintCallable, Category = Properties)
		void OnInventoryItemDragAndDropNotify(int index, FVector2D vPos);

	UFUNCTION(BlueprintCallable, Category = Properties)
		void OnInventoryItemCancelNotify(int index);

	UFUNCTION(BlueprintCallable, Category = Properties)
		UTexture2D* GetCurrentSelectedBagItemImage();

	UFUNCTION(BlueprintCallable, Category = Properties)
		FString GetCurrentSelectedBagItemName();

	UFUNCTION(BlueprintCallable, Category = Properties)
		FString GetCurrentSelectedBagItemHelp();

	UFUNCTION(BlueprintCallable, Category = Properties)
		bool CheckBagTitleClickedState(FVector2D vPos);

	UFUNCTION(BlueprintCallable, Category = Properties)
		USkeletalMesh* GetHeroEquipMesh(EQUIP_ITEM_TYPE type);

	UFUNCTION(BlueprintCallable, Category = Properties)
		USkeletalMesh* GetHeroWeaponMesh();

	UFUNCTION(BlueprintCallable, Category = Properties)
		FName GetWeaponSocketName();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainProp")
		int32 curCoinCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainProp")
		FVector2D bagPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainProp")
		FVector2D bagOffsetPosition;

	UFUNCTION(BlueprintCallable, Category = Properties)
		FName GetCurrentSelectedHeroName();

	UFUNCTION(BlueprintCallable, Category = Properties)
		int32 GetCurrentSelectedSkillIndex();

private:
	//Input
	void OnInputPressed();
	void OnInputReleased();
	void OnKeyPressed_1();
	void OnKeyPressed_2();
	void OnKeyPressed_3();
	void OnKeyPressed_Up();
	void OnKeyPressed_Down();
	void OnKeyPressed_Left();
	void OnKeyPressed_Right();
	void OnKeyReleased_Direction();
	void UpdateInputData(float deltaSeconds);
	bool CheckCursorRangeSelectState();

	//HeroCard
	void AddHeroCard();
	void UpdateAllHeroCardsData();
	int GetSelectedHeroCard(int x, int y);
	void UpdateSpawingCursorToWorld();
	void OnEnableHeroCard(int index, FVector vPos);
	void OnSelectHeroCard(int index);

    //Agent
	void UpdateAgentCharactersList();
	bool CheckPressedOnAgent(AActor* pActor);
	bool CheckPressedOnEnemy(AActor* pActor);
	void SingleSelectHero(AHeroAgent* pHero);
	void UpdateAgentCharactersSelectedState();
	void OnPressedOnEnemy(AEnemyAgent* pPressedEnemy);
	void OnPressedOnHero(AHeroAgent* pPressedHero);

	//Inventory
	void InitInventory();
	void OnToolBarItemClicked(int index);
	bool AddDropObjectToInventory(ADropObject* pDropObject);

	//Camera
	void InitCamera();
	void MoveCameraTo(CAMERA_MOVE_DIRECTION_TYPE type, float deltaSeconds);
	void MoveCameraTo(AAgent* pAgent);
	void UpdateCamera(GAME_STATE state, float deltaSeconds);
	void OnCameraZoomIn();
	void OnCameraZoomOut();

	//Skill
	void OnSwitchSkill(int index);

	//Role Switch
	void SwitchHero(int rotAngle);
	void SetCurrentSwitchedHeroIndex(int index);
	void UpdateSwitchHeroes(float DeltaSeconds, bool bForce);

	void ShowMoveTarget(bool bState, FVector vPos);
	int RotateTo(int startAngle, int targetAngle, int moveRotAngle);

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	TSubclassOf<AActor> m_pMoveTargetActorClass;
	AActor* m_pMoveTargetActor;
	UDecalComponent* m_pSpawingCursorToWorld;
	AGamePlayerController* m_pPlayerController;
	AKiLiGameMode* m_pGameMode;
	AObjectItemManager* m_pObjectItemManager;
	Inventory m_inventory;
	HeroCardData m_heroCardData;
	UserInputData m_inputData;
	CameraData m_cameraData;
	AgentGroupData m_agentGroupData;
	RoleSwitchData m_roleSwitchData;
};
