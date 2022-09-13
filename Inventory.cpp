#include "Inventory.h"
#include "AgentsManager.h"
#include "ObjectItemManager.h"
 
#define MAX_ITEM_STACK_COUNT 99

Inventory::Inventory()
{
	for (int i = 0; i < MAX_ITEM_COUNT; i++)
	{
		ObjectItemInfo item;
		m_itemInfoArray.Add(item);
	}

	m_iSelectedBagItemIndex = -1;
}

void Inventory::Init(AAgentsManager* pHeroesManager, AObjectItemManager* pObjectItemManager)
{
	m_pAgentsManager = pHeroesManager;
	m_pObjectItemManager = pObjectItemManager;
}

bool Inventory::AddObjectItemToBag(FName id, int count)
{
	return AddObjectItemToBag(id, 0, MAX_BAG_ITEM_COUNT, count);

//	if (bResult)
//		m_pCharacter->AddRewardData(id, count);
}

void Inventory::AddObjectItemToToolBar(int index, FName id)
{
	m_itemInfoArray[TOOLBAR_ITEM_INDEX + index].id = id;
	m_itemInfoArray[TOOLBAR_ITEM_INDEX + index].count = 1;
}

bool Inventory::AddObjectItemToBag(FName id, int startIndex, int endIndex, int& leftCount)
{
	ObjectItem* pObjectItem = m_pObjectItemManager->GetObjectItem(id);
	if (pObjectItem)
	{
		bool bStack = pObjectItem->GetStackState();
		for (int i = startIndex; i < endIndex; i++)
		{
			if (m_itemInfoArray[i].id == id)
			{
				if (bStack)
				{
					int canPutCount = MAX_ITEM_STACK_COUNT - m_itemInfoArray[i].count;
					if (leftCount <= canPutCount)
					{
						m_itemInfoArray[i].count += leftCount;
						return true;
					}
					else
					{
						leftCount -= canPutCount;
						m_itemInfoArray[i].count += canPutCount;
					}
				}
			}
		}

		if (startIndex < MAX_BAG_ITEM_COUNT)
		{
			for (int i = startIndex; i < endIndex; i++)
			{
				if (m_itemInfoArray[i].id == "none")
				{
					int canPutCount = (bStack)? MAX_ITEM_STACK_COUNT : 1;
					m_itemInfoArray[i].id = id;
					if (leftCount <= canPutCount)
					{
						m_itemInfoArray[i].count += leftCount;
						return true;
					}
					else
					{
						leftCount -= canPutCount;
						m_itemInfoArray[i].count += canPutCount;
					}
				}
			}
		}
	}

	return false;
}

void Inventory::AddObjectItemToEquip(FName id)
{
	ObjectItem* pObjectItem = m_pObjectItemManager->GetObjectItem(id);
 	if (pObjectItem && pObjectItem->type >= OBJECT_ITEM_EQUIP_TOP_BODY)
	{
		int index = EQUIP_ITEM_INDEX + (pObjectItem->type - OBJECT_ITEM_EQUIP_TOP_BODY);
		m_itemInfoArray[index].id = id;
		m_itemInfoArray[index].count = 1;
	}
}

bool Inventory::ReduceObjectItem(FName id)
{
	int index = -1;
	for (int i = TOOLBAR_ITEM_INDEX; i < (TOOLBAR_ITEM_INDEX + MAX_TOOLBAR_ITEM_COUNT); i++)
	{
		if (m_itemInfoArray[i].id == id)
		{
			index = i;
			break;
		}
	}

	if (index == -1)
	{
		for (int i = 0; i < MAX_BAG_ITEM_COUNT; i++)
		{
			if (m_itemInfoArray[i].id == id)
			{
				index = i;
				break;
			}
		}
	}

	if (index >= 0)
	{
		ReduceObjectItem(index);
		return true;
	}

	return false;
}

void Inventory::ReduceObjectItem(int index)
{
	m_itemInfoArray[index].count -= 1;
	if (m_itemInfoArray[index].count == 0)
		ClearItem(index);
}

ObjectItemInfo* Inventory::GetBagItemInfo(int index)
{ 
	return (index >= 0 && index < MAX_BAG_ITEM_COUNT)? &m_itemInfoArray[index] : nullptr;
}

ObjectItemInfo* Inventory::GetToolBarItemInfo(int index)
{
	return (index < MAX_TOOLBAR_ITEM_COUNT)? &m_itemInfoArray[TOOLBAR_ITEM_INDEX + index] : nullptr;
}

ObjectItemInfo* Inventory::GetEquipItemInfo(int index)
{
	return (index < MAX_EQUIP_ITEM_COUNT)? &m_itemInfoArray[EQUIP_ITEM_INDEX + index] : nullptr;
}

ObjectItemInfo* Inventory::GetWeaponItemInfo()
{
	return &m_itemInfoArray[WEAPON_ITEM_INDEX];
}

ObjectItemInfo* Inventory::GetItemInfo(int index)
{
	if (m_itemInfoArray.Num() > 0)
	{
		if (index >= 0 && index < MAX_ITEM_COUNT)
			return &m_itemInfoArray[index];
	}

	return nullptr;
}

void Inventory::ClearToolBarItem(int index)
{
	m_itemInfoArray[TOOLBAR_ITEM_INDEX + index].Clear();
}

FName Inventory::GetToolBarItemName(int index)
{
	return m_itemInfoArray[TOOLBAR_ITEM_INDEX + index].id;
}

void Inventory::OnDragAndDropNotify(int srcIndex, FVector2D vPos)
{
	if (m_itemInfoArray[srcIndex].id == "none")
		return;

	int targetIndex = GetTargetItemIndex(vPos);
	if (targetIndex >= 0)
	{
		ObjectItem* pSrcObjectItem = m_pObjectItemManager->GetObjectItem(m_itemInfoArray[srcIndex].id);
		OBJECT_ITEM_TYPE targetType;
		bool bNeedUpdateEquipItem = false;

		if (m_itemInfoArray[targetIndex].id == "none")
			targetType = (OBJECT_ITEM_TYPE)(targetIndex - EQUIP_ITEM_INDEX + OBJECT_ITEM_EQUIP_TOP_BODY);
		else
			targetType = m_pObjectItemManager->GetObjectItem(m_itemInfoArray[targetIndex].id)->type;

		if (targetIndex < 0 && targetIndex >= MAX_ITEM_COUNT)
			ClearItem(srcIndex);
		else if (targetIndex >= EQUIP_ITEM_INDEX)
		{
			if (pSrcObjectItem->type == targetType)
			{
				SwapItem(srcIndex, targetIndex);
				bNeedUpdateEquipItem = true;
			}
		}
		else if (targetIndex >= TOOLBAR_ITEM_INDEX && targetIndex < (TOOLBAR_ITEM_INDEX + MAX_TOOLBAR_ITEM_COUNT))
		{
			if (CheckItemCanAddToToolBar(pSrcObjectItem->type))
				SwapItem(srcIndex, targetIndex);
		}
		else if (targetIndex >= 0 && targetIndex < MAX_BAG_ITEM_COUNT)
		{
			if (m_itemInfoArray[targetIndex].id == "none")
			{
				SwapItem(srcIndex, targetIndex);
				if (pSrcObjectItem->type >= OBJECT_ITEM_EQUIP_TOP_BODY)
					bNeedUpdateEquipItem = true;
			}
			else if (srcIndex >= TOOLBAR_ITEM_INDEX && srcIndex < (TOOLBAR_ITEM_INDEX + MAX_TOOLBAR_ITEM_COUNT))
			{
				if (CheckItemCanAddToToolBar(targetType))
					SwapItem(srcIndex, targetIndex);
			}
			else if (srcIndex >= 0 && srcIndex < MAX_BAG_ITEM_COUNT)
				SwapItem(srcIndex, targetIndex);
		}

 		if (bNeedUpdateEquipItem)
			m_pAgentsManager->OnUpdateEquipItem();
	}
}

bool Inventory::CheckItemCanAddToToolBar(OBJECT_ITEM_TYPE type)
{
	return (type >= OBJECT_ITEM_FOOD && type <= OBJECT_ITEM_MATERIAL)? true : false;
}

int Inventory::GetTargetItemIndex(FVector2D vPos)
{
	int itemIndex = -1;
	FVector2D vScale = FVector2D(1, 1);// m_pGameMode->GetResolutionRate();
	int typeIndex = -1;
	int frameX = 6 * vScale.X;
	int frameY = 6 * vScale.Y;
	int iconSizeX[2] = {64 * vScale.X, 96 * vScale.X};
	int iconSizeY[2] = {64 * vScale.Y, 96 * vScale.Y};
	int bagItemPosX = BAG_ITEM_POS.X * vScale.X;
	int bagItemPosY = BAG_ITEM_POS.Y * vScale.Y;
	int toolBarItemPosX = TOOLBAR_ITEM_POS.X * vScale.X;
	int toolBarItemPosY = TOOLBAR_ITEM_POS.Y * vScale.Y;
	int startX[2] = { m_pAgentsManager->bagPosition.X + bagItemPosX, toolBarItemPosX};
	int startY[2] = { m_pAgentsManager->bagPosition.Y + bagItemPosY, toolBarItemPosY};
	int width[2] = {iconSizeX[0] * BAG_ITEM_X_COUNT + (BAG_ITEM_X_COUNT - 1) * frameX, 
		            iconSizeX[1] * MAX_TOOLBAR_ITEM_COUNT + (MAX_TOOLBAR_ITEM_COUNT - 1) * frameX};
	int height[2] = {iconSizeY[0] * BAG_ITEM_Y_COUNT + (BAG_ITEM_Y_COUNT - 1) * frameY, iconSizeY[1]};
	int xCount[2] = {BAG_ITEM_X_COUNT, MAX_TOOLBAR_ITEM_COUNT};
	for (int i = 0; i < 2; i++)
	{
		if (vPos.X >= startX[i] && vPos.X <= (startX[i] + width[i]) &&
			vPos.Y >= startY[i] && vPos.Y <= (startY[i] + height[i]))
		{
			typeIndex = i;
			break;
		}
	}

	if (typeIndex >= 0 && typeIndex <= 1)
	{
		int xIndex = (vPos.X - startX[typeIndex]) / (iconSizeX[typeIndex] + frameX);
		int yIndex = (vPos.Y - startY[typeIndex]) / (iconSizeY[typeIndex] + frameY);
		itemIndex = typeIndex * EQUIP_ITEM_INDEX + yIndex * xCount[typeIndex] + xIndex;
	}
	else
	{
		int equipItemPosX[3] = {216, 216, 216};
		int equipItemPosY[3] = {60, 200, 288};
		int index[3] = {0, 1, 6};
		for (int i = 0; i < 3; i++)
		{
			int xPos = m_pAgentsManager->bagPosition.X + equipItemPosX[i] * vScale.X;
			int yPos = m_pAgentsManager->bagPosition.Y + equipItemPosY[i] * vScale.Y;
			if (vPos.X >= xPos && vPos.X <= (xPos + iconSizeX[i]) &&
				vPos.Y >= yPos && vPos.Y <= (yPos + iconSizeY[i]))
				return (EQUIP_ITEM_INDEX + index[i]);
		}
	}

	return itemIndex;
}

void Inventory::SwapItem(int srcIndex, int destIndex)
{
	ObjectItemInfo tempItem;
	tempItem.CopyFrom(m_itemInfoArray[srcIndex]);
	m_itemInfoArray[srcIndex].CopyFrom(m_itemInfoArray[destIndex]);
	m_itemInfoArray[destIndex].CopyFrom(tempItem);
}

void Inventory::Clear()
{
	m_itemInfoArray.Empty();
}

void Inventory::ClearItem(int index)
{
	m_itemInfoArray[index].Clear();
}

bool Inventory::AddItems(FName id1, FName id2)
{
	TArray<ObjectItemInfo> itemInfoArray;
	for (int i = 0; i < m_itemInfoArray.Num(); i++)
	{
		ObjectItemInfo info;
		info.CopyFrom(m_itemInfoArray[i]);
		itemInfoArray.Add(info);
	}

	if (AddObjectItemToBag(id1))
	{
		if (AddObjectItemToBag(id2))
			return true;
	}

	for (int i = 0; i < m_itemInfoArray.Num(); i++)
		m_itemInfoArray[i].CopyFrom(itemInfoArray[i]);
	
	return false;
}

void Inventory::OnBagtemClickedNotify(int index)
{
	if (m_pAgentsManager->UseObjectItem(m_itemInfoArray[index].id))
		ReduceObjectItem(index);
	else
		m_iSelectedBagItemIndex = index;
}

ObjectItemInfo* Inventory::GetCurrentSelectedBagItemInfo()
{
	return GetBagItemInfo(m_iSelectedBagItemIndex);
}

EQUIP_ITEM_TYPE Inventory::GetEquipObjectItem(int index)
{
	if (index >= EQUIP_ITEM_INDEX)
		return (EQUIP_ITEM_TYPE)(index - EQUIP_ITEM_INDEX);

	return EQUIP_ITEM_TYPE::NONE;
}