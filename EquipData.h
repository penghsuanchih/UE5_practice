#pragma once
#include "CoreMinimal.h"
 
UENUM(BlueprintType)
enum class EQUIP_ITEM_TYPE : uint8
{
    TOP_BODY = 0,
    BOT_BODY,
    HAND,
    HEAD,
    HAIR,
    SHOE,
    COUNT,
    NONE
};


struct EquipData
{
    EquipData()
    {
        defaultName = "none";
        name = "none";
        m_pComp = nullptr;
    }

    FName defaultName;
    FName name;
    USkeletalMeshComponent* m_pComp;
};

