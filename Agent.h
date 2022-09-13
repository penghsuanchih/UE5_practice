#pragma once

#include "CoreMinimal.h"
#include "AgentData.h"
#include "EquipData.h"
#include "GameFramework/Character.h"
#include "Agent.generated.h"

class AAgentsManager;
class AAgentPlayerController;
class ASkillManager;
UCLASS(Blueprintable)
class AAgent : public ACharacter
{
	GENERATED_BODY()

public:
	AAgent();
	virtual void BeginPlay() override;
	virtual void Tick(float deltaSeconds) override;
	AgentData* GetData() {return &m_data;}
	AGENT_GROUP_TYPE GetGroupType() {return m_data.groupType;}
	void MoveTo(FVector vPos);
	void SetSelectedState(bool state) {m_data.bSelectedState = state;}
	void OnNotifyAnimEnd();
	void OnNotifyMoveStop();
	virtual void OnDestroy();
	void AttackTo(AAgent* pHeroCharacter);
	void TakeAttackDamage(float damage, AAgent* pEnemy, bool hurtOverlay, FVector vPos);
	AAgent* GetTargetEnemy() {return m_pTargetEnemy;}
	void SetTargetEnemy(AAgent* pAgent) {m_pTargetEnemy = pAgent;}
	void SetAnimActionType(AGENT_ANIM_ACTION_TYPE animType) {m_data.animActionType = animType;}
	void SetCurrentSkillType(SKILL_TYPE type) {m_data.curSkillType = type;}
	void SetCurrentAttackIndex(int index) {m_data.curAttackIndex = index;}
	bool GetComboAttackSate() {return m_data.bComboAttack;}
	void SetComboAttackSate(bool state) {m_data.bComboAttack = state;}
	void OnSwitchSkill(int index);
	long GetHP() {return m_data.curHP;}
	bool IsEnemy(AAgent* pAgent);
	void UpdateRenderCustomDepthState(FVector vCameraPos);
	bool CheckLineDetectState(FVector vCameraPos);
	virtual void StartAttack() {}
	void OnNotifiedAgentDied(AAgent* pAgent);
	virtual FVector GetWeaponSocketPosition() {return FVector::ZeroVector;}
	FVector GetWeaponAttackSocketPosition(FName name);
	FName GetWeaponSocketName();
	virtual void SetWeapon(FName name) {}
	USkeletalMesh* GetWeponMesh();
	void OnAttackBegin(SKILL_TYPE skillType);
	void AddHP(long hp);

	UFUNCTION(BlueprintCallable, Category = Properties)
		AGENT_ANIM_ACTION_TYPE GetAnimActionState() {return m_data.animActionType;}

	UFUNCTION(BlueprintCallable, Category = Properties)
		SKILL_TYPE GetCurrentSkillType() {return m_data.curSkillType;}

	UFUNCTION(BlueprintCallable, Category = Properties)
		int GetCurrentAttackIndex() {return m_data.curAttackIndex;}

	UFUNCTION(BlueprintCallable, Category = Properties)
		HERO_ROLE_TYPE GetRoleType() {return m_data.roleType;}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* DecalComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainProp")
		class USkeletalMeshComponent* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainProp")
		int32 maxHP;

protected:
	void OnDeath();
	void UpdateDecalComponentState(); 
	bool AttackRotate(float deltaSeconds);
	void SetDefaultWeaponItem(FName name);
	FName GetWeaponSocketName(FName name);
 
	AAgentPlayerController* m_pPlayerController;
	AAgentsManager* m_pAgentsManager;
	AgentData m_data;
	AAgent* m_pTargetEnemy;
	ASkillManager* m_pSkillManager;
	EquipData m_weapon;
};

 