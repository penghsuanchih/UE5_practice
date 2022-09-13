#include "AgentsManager.h"
#include "KiLiGameMode.h"
#include "ObjectItemManager.h"
#include "DropObject.h"

void AAgentsManager::InitInventory()
{
	curCoinCount = 0;
	bagPosition = BAG_TITLE_POS;
	m_inventory.Init(this, m_pObjectItemManager);
}

UTexture2D* AAgentsManager::GetInventoryItemImage(int index)
{
 	ObjectItemInfo* pInfo = m_inventory.GetItemInfo(index);
 	FName id = (pInfo)? pInfo->id : "none";
	if (m_pObjectItemManager)
 		return m_pObjectItemManager->GetObjectItem(id)->sprite;

	return nullptr;
}

UTexture2D* AAgentsManager::GetInventoryItemBgImage(int index)
{
	FName id = "none_bg";
	if (index >= EQUIP_ITEM_INDEX) 
	{
		FName idList[] = {"topBody_bg", "botBody_bg", "none_bg", "none_bg", "none_bg", "none_bg", "none_bg"};
		id = idList[index - EQUIP_ITEM_INDEX];
	}

	ObjectItem* pObjectItem = m_pObjectItemManager->GetObjectItem(id);
	return (pObjectItem)? pObjectItem->sprite : nullptr;
}

int AAgentsManager::GetInventoryItemCount(int index)
{
    ObjectItemInfo* pInfo = m_inventory.GetItemInfo(index);
	return (pInfo)? pInfo->count : 0;
}

void AAgentsManager::OnInventoryItemClickedNotify(int index)
{
 	if (index >= MAX_BAG_ITEM_COUNT)
 		OnToolBarItemClicked(index);
 	else
 		m_inventory.OnBagtemClickedNotify(index);
}

void AAgentsManager::OnToolBarItemClicked(int index)
{
	FName szItemName = m_inventory.GetItemInfo(index)->id;
	ObjectItem* pObjectItem = m_pObjectItemManager->GetObjectItem(szItemName);
	if (pObjectItem)
	{
		//if (pObjectItem->type == OBJECT_ITEM_NONE) {}
			//CancelHandItem();
		//else if (pObjectItem->type >= OBJECT_ITEM_TOOL && pObjectItem->type <= OBJECT_ITEM_WEAPON)
		//	OnEquipHandItem(szItemName);
	}
}

void AAgentsManager::OnInventoryItemCancelNotify(int index)
{
	m_inventory.ClearItem(index);
}

void AAgentsManager::OnInventoryItemDragAndDropNotify(int index, FVector2D vPos)
{
	m_inventory.OnDragAndDropNotify(index, vPos);
}

UTexture2D* AAgentsManager::GetCurrentSelectedBagItemImage()
{
	ObjectItemInfo* pInfo = m_inventory.GetCurrentSelectedBagItemInfo();
	if (pInfo && pInfo->id != "none")
	{
		ObjectItem* pObjectItem = m_pObjectItemManager->GetObjectItem(pInfo->id);
		if (pObjectItem)
			return pObjectItem->sprite;
    }

	return nullptr;
}

FString AAgentsManager::GetCurrentSelectedBagItemName()
{
//	ObjectItemInfo* pInfo = m_inventory.GetCurrentSelectedBagItemInfo();
//if (pInfo)
//		return m_pCommonResource->GetObjectTextName(pInfo->id);

	return "none";
}

FString AAgentsManager::GetCurrentSelectedBagItemHelp()
{
//	ObjectItemInfo* pInfo = m_inventory.GetCurrentSelectedBagItemInfo();
//	if (pInfo)
//		return m_pObjectItemManager->GetObjectTextHelp(pInfo->id);

	return "none";
}

bool AAgentsManager::CheckBagTitleClickedState(FVector2D vPos)
{
	FVector2D vScale = FVector2D(1, 1);// pGameMode->GetResolutionRate();
	int toolBarHeight = BAG_TITLE_HEIGHT * vScale.Y;
	if (vPos.Y >= bagPosition.Y && vPos.Y <= (bagPosition.Y + toolBarHeight))
			return true;

	return false;
}

bool AAgentsManager::AddDropObjectToInventory(ADropObject* pDropObject)
{
	if (m_inventory.AddObjectItemToBag(pDropObject->GetObjectName()))
	{
		pDropObject->Destroy();
		return true;
	}

	return false;
}

bool AAgentsManager::UseObjectItem(FName name)
{
	if (name == "potion")
	{
		if (m_agentGroupData.selectedAgentsList.Num())
		{
			AHeroAgent* pHeroAgent = m_agentGroupData.selectedAgentsList[0];
			m_pGameMode->SpawnEffectParticle(VISIBLE_EFFECT_TYPE::HEAL, pHeroAgent->GetActorLocation());
			pHeroAgent->AddHP(100);
		    return true;
		}
	}

	return false;
}