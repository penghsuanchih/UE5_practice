#pragma once
#include "Engine/StaticMeshActor.h"
#include "Animation/SkeletalMeshActor.h"
#include "UObject/NameTypes.h"
//#include "PaperSpriteComponent.h"


enum OBJECT_ITEM_TYPE
{
	OBJECT_ITEM_NONE = 0,
	OBJECT_ITEM_FOOD,
	OBJECT_ITEM_TOOL,
	OBJECT_ITEM_RESOURCE_TOOL,
	OBJECT_ITEM_MATERIAL,
	OBJECT_ITEM_EQUIP_TOP_BODY,
	OBJECT_ITEM_EQUIP_BOT_BODY,
	OBJECT_ITEM_EQUIP_HAND,
	OBJECT_ITEM_EQUIP_HEAD,
	OBJECT_ITEM_EQUIP_HAIR,
	OBJECT_ITEM_EQUIP_SHOE,
	OBJECT_ITEM_EQUIP_WEAPON,
	OBJECT_ITEM_SKILL
};

struct ObjectItem
{
	ObjectItem()
	{
		sprite = nullptr;
		staticmesh = nullptr;
		skeletalMesh = nullptr;
		type = OBJECT_ITEM_NONE;
	}

	bool GetStackState()
	{
		if (type == OBJECT_ITEM_FOOD)
			return true;

		return false;
	}

	UTexture2D* sprite;
	UStaticMesh* staticmesh;
	USkeletalMesh* skeletalMesh;
	OBJECT_ITEM_TYPE type;
};
