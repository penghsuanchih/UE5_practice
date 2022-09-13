#include "HeroAgent.h"
#include "AgentsManager.h"
#include "SkillManager.h"

AHeroAgent::AHeroAgent()
{
	m_equipData[int(EQUIP_ITEM_TYPE::TOP_BODY)].m_pComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("equip_top_body"));
	m_equipData[int(EQUIP_ITEM_TYPE::BOT_BODY)].m_pComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("equip_bot_body"));
	m_equipData[int(EQUIP_ITEM_TYPE::HAND)].m_pComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("equip_hand"));
	m_equipData[int(EQUIP_ITEM_TYPE::HEAD)].m_pComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("equip_head"));
	m_equipData[int(EQUIP_ITEM_TYPE::HAIR)].m_pComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("equip_hair"));
	m_equipData[int(EQUIP_ITEM_TYPE::SHOE)].m_pComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("equip_shoe"));
	m_data.groupType = AGENT_GROUP_TYPE::PLAYER;
}
 
void AHeroAgent::BeginPlay()
{
	Super::BeginPlay();
 
}

void AHeroAgent::SetWeapon(FName name)
{
	ObjectItem* pItem = m_pAgentsManager->GetObjectItem(name);
	m_weapon.name = name;
	if (pItem && pItem->type == OBJECT_ITEM_EQUIP_WEAPON)
	{
		FName szSocketName = GetWeaponSocketName(name);
		m_weapon.m_pComp->SetSkeletalMesh(pItem->skeletalMesh);
		m_weapon.m_pComp->AttachToComponent(m_equipData[int(EQUIP_ITEM_TYPE::HAND)].m_pComp, FAttachmentTransformRules::KeepRelativeTransform, szSocketName);
	}
	else
		m_weapon.m_pComp->SetSkeletalMesh(nullptr);
}

void AHeroAgent::SetDefaultEquipItem(EQUIP_ITEM_TYPE type, FName name)
{
	m_equipData[int(type)].defaultName = name;
	SetEquipItem(type, name);
}

void AHeroAgent::SetEquipItem(EQUIP_ITEM_TYPE type, FName name)
{
	FName equipName = (name != "none")? name : m_equipData[int(type)].defaultName;
	ObjectItem* pItem = m_pAgentsManager->GetObjectItem(equipName);
	if (pItem)
	{
		m_equipData[int(type)].m_pComp->AttachToComponent(nullptr, FAttachmentTransformRules::KeepRelativeTransform);
		if (pItem->type == OBJECT_ITEM_EQUIP_HAIR)
			m_equipData[int(type)].m_pComp->AttachToComponent(m_equipData[int(EQUIP_ITEM_TYPE::HEAD)].m_pComp, FAttachmentTransformRules::KeepRelativeTransform, "HAIR");
		else
			m_equipData[int(type)].m_pComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

		m_equipData[int(type)].name = equipName;
		m_equipData[int(type)].m_pComp->SetSkeletalMesh(pItem->skeletalMesh, false);
		m_equipData[int(type)].m_pComp->SetAnimInstanceClass(m_pAnimClass);
	}
 
}

USkeletalMesh* AHeroAgent::GetEquipMesh(EQUIP_ITEM_TYPE type)
{
	return m_equipData[int(type)].m_pComp->SkeletalMesh;
}

FVector AHeroAgent::GetWeaponSocketPosition()
{
	FName name = GetWeaponSocketName();
	return m_equipData[int(EQUIP_ITEM_TYPE::HAND)].m_pComp->GetSocketLocation(name);
}

 
