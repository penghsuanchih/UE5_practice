#pragma once
#include "ObjectItem.h"
#include "EquipData.h"
#include "UObject/NameTypes.h"

#define MAX_BAG_ITEM_COUNT 20
#define MAX_TOOLBAR_ITEM_COUNT 10
#define MAX_EQUIP_ITEM_COUNT 6
#define MAX_WEAPON_ITEM_COUNT 1
#define BAG_ITEM_X_COUNT 5
#define BAG_ITEM_Y_COUNT 4
#define TOOLBAR_ITEM_INDEX MAX_BAG_ITEM_COUNT
#define EQUIP_ITEM_INDEX (TOOLBAR_ITEM_INDEX + MAX_TOOLBAR_ITEM_COUNT)
#define WEAPON_ITEM_INDEX (EQUIP_ITEM_INDEX + MAX_EQUIP_ITEM_COUNT)
#define MAX_ITEM_COUNT (WEAPON_ITEM_INDEX + MAX_WEAPON_ITEM_COUNT)
#define BAG_TITLE_POS FVector2D(1200, 500)
#define BAG_ITEM_POS FVector2D(305, 30)
#define TOOLBAR_ITEM_POS FVector2D(417, 952)
#define BAG_TITLE_HEIGHT 27
 

struct ObjectItemInfo
{
    ObjectItemInfo()
    {
        Clear();
    }

    void Clear()
    {
        count = 0;
        id = TEXT("none");
    }

    void CopyFrom(ObjectItemInfo item)
    {
        id = item.id;
        count = item.count;
    }

    FName id;
    int count;
};

class AAgentsManager;
class AObjectItemManager;
class Inventory
{
public:
    Inventory();
    void Init(AAgentsManager* pAgentsManager, AObjectItemManager* pObjectItemManager);
 	ObjectItemInfo* GetBagItemInfo(int index);
    ObjectItemInfo* GetToolBarItemInfo(int index);
    ObjectItemInfo* GetEquipItemInfo(int index);
    ObjectItemInfo* GetWeaponItemInfo();
    ObjectItemInfo* GetItemInfo(int index);
 	bool AddObjectItemToBag(FName id, int count = 1);
    void AddObjectItemToToolBar(int index, FName id);
    void AddObjectItemToEquip(FName id);
    bool ReduceObjectItem(FName id);
    void ReduceObjectItem(int index);
    FName GetToolBarItemName(int index);
    void ClearToolBarItem(int index);
    void OnDragAndDropNotify(int srcIndex, FVector2D vPos);
    void ClearItem(int index);
    bool AddItems(FName id1, FName id2);
    void Clear();
    void OnBagtemClickedNotify(int index);
    ObjectItemInfo* GetCurrentSelectedBagItemInfo();
    bool CheckItemCanAddToToolBar(OBJECT_ITEM_TYPE type);
    EQUIP_ITEM_TYPE GetEquipObjectItem(int index);

private:
    int GetTargetItemIndex(FVector2D vPos);
    void SwapItem(int srcIndex, int destIndex);
    bool AddObjectItemToBag(FName id, int startIndex, int endIndex, int& leftCount);

    AAgentsManager* m_pAgentsManager;
    AObjectItemManager* m_pObjectItemManager;
 	TArray<ObjectItemInfo> m_itemInfoArray;
    int m_iSelectedBagItemIndex;
};