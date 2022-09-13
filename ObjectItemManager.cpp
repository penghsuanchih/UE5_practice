#include "ObjectItemManager.h"


UTexture2D* LoadTextureFromPath(const FString& Path)
{
	return Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *(Path)));
}

UStaticMesh* LoadStaticMeshFromPath(const FString& Path)
{
	return Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *(Path)));
}

USkeletalMesh* LoadSkeletalMeshFromPath(const FString& Path)
{
	return Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), NULL, *(Path)));
}

AObjectItemManager::AObjectItemManager()
{
	AddData("none", "null", "none", OBJECT_ITEM_NONE);
	AddData("none_bg", "T_Cell_01", "none", OBJECT_ITEM_NONE);
	AddData("claw", "claw", "Hook/hook.hook", OBJECT_ITEM_TOOL);
	AddData("hammer", "hammer", "Hammer/hammer.hammer", OBJECT_ITEM_TOOL);
	AddData("bow", "bow", "Bow/bow.bow", OBJECT_ITEM_EQUIP_WEAPON);
	AddData("sword", "sword", "Sword/sword.sword", OBJECT_ITEM_EQUIP_WEAPON);
	AddData("sword2", "sword2", "Sword/sword2.sword2", OBJECT_ITEM_EQUIP_WEAPON);
	AddData("gun", "gun", "Gun/gun.gun", OBJECT_ITEM_EQUIP_WEAPON);
	AddData("fishingrods", "fishingrods", "FishingRod/fishingRod.fishingRod", OBJECT_ITEM_TOOL);
	AddData("axe", "axe", "Axe/axe.axe", OBJECT_ITEM_RESOURCE_TOOL);
	AddData("pickaxe", "pickaxe", "Pickaxe/pickaxe.pickaxe", OBJECT_ITEM_RESOURCE_TOOL);
	AddData("wood", "wood", "Wood/wood.wood", OBJECT_ITEM_MATERIAL);
	AddData("plastic", "plastic", "Plastic/plastic.plastic", OBJECT_ITEM_MATERIAL);
	AddData("flowerpot", "flowerpot", "PlantPot/plantPot.plantPot", OBJECT_ITEM_MATERIAL);
	AddData("flower", "plant_potato", "Paint/Potato/potato.potato", OBJECT_ITEM_MATERIAL);
	AddData("arrow", "none", "Arrow/arrow.arrow", OBJECT_ITEM_NONE);
	AddData("fish", "fish", "none", OBJECT_ITEM_FOOD);
	AddData("raftcell", "none", "RaftCell/raftCell.raftCell", OBJECT_ITEM_NONE);
	AddData("raftcell_sel", "none", "RaftCell/raftCell_sel.raftCell_sel", OBJECT_ITEM_NONE);
	AddData("coin", "coin", "none", OBJECT_ITEM_NONE);
	AddData("potato", "potato", "none", OBJECT_ITEM_FOOD);
	AddData("stone", "stone", "Stone/stone.stone", OBJECT_ITEM_MATERIAL);
	AddData("leaf", "leaf", "Leaf/leaf.leaf", OBJECT_ITEM_MATERIAL);
	AddData("apple", "apple", "Apple/apple.apple", OBJECT_ITEM_FOOD);
	AddData("potion", "potion", "none", OBJECT_ITEM_FOOD);
	AddData("hambone", "hambone", "Hambone/hambone.hambone", OBJECT_ITEM_FOOD);
	AddData("topBody_bg", "T_ShirtEquip", "none", OBJECT_ITEM_EQUIP_TOP_BODY);
	AddData("botBody_bg", "T_PantsEquip", "none", OBJECT_ITEM_EQUIP_BOT_BODY);
	AddData("A_TopBody_A", "A_TopBody_A", "TopBodies/A_TopBody_A.A_TopBody_A", OBJECT_ITEM_EQUIP_TOP_BODY);
	AddData("A_TopBody_B", "A_TopBody_B", "TopBodies/A_TopBody_B.A_TopBody_B", OBJECT_ITEM_EQUIP_TOP_BODY);
	AddData("B_TopBody_A", "B_TopBody_A", "TopBodies/B_TopBody_A.B_TopBody_A", OBJECT_ITEM_EQUIP_TOP_BODY);
	AddData("C_TopBody_A", "C_TopBody_A", "TopBodies/C_TopBody_A.C_TopBody_A", OBJECT_ITEM_EQUIP_TOP_BODY);
	AddData("A_BotBody_A", "A_BotBody_A", "BotBodies/A_BotBody_A.A_BotBody_A", OBJECT_ITEM_EQUIP_BOT_BODY);
	AddData("A_BotBody_B", "A_BotBody_B", "BotBodies/A_BotBody_B.A_BotBody_B", OBJECT_ITEM_EQUIP_BOT_BODY);
	AddData("A_hand", "none", "Gloves/A_Hands.A_Hands", OBJECT_ITEM_EQUIP_HAND);
	AddData("A_head", "none", "Heads/A_Head.A_Head", OBJECT_ITEM_EQUIP_HEAD);
	AddData("A_hair", "none", "HairsHats/A_Hair_B.A_Hair_B", OBJECT_ITEM_EQUIP_HAIR);
	AddData("A_shoe", "none", "Shoes/A_shoe.A_shoe", OBJECT_ITEM_EQUIP_SHOE);
	AddData("skill_1", "skill_1", "none", OBJECT_ITEM_SKILL);
	AddData("skill_2", "skill_2", "none", OBJECT_ITEM_SKILL);
	AddData("skill_3", "skill_3", "none", OBJECT_ITEM_SKILL);
}

void AObjectItemManager::AddData(FName id, FString szPicName, FString szMeshName, OBJECT_ITEM_TYPE type)
{
	ObjectItem item;
	if (szPicName != "none")
	{
		FString szPicPath = "Texture2D'/Game/UI/texture/" + szPicName + "." + szPicName + "'";
		item.sprite = LoadTextureFromPath(szPicPath);
	}

	if (szMeshName != "none")
	{
		if (type == OBJECT_ITEM_EQUIP_WEAPON)
		{
			FString szMeshPath = "SkeletalMesh'/Game/Item/Weapon/" + szMeshName + "'";
			item.skeletalMesh = LoadSkeletalMeshFromPath(szMeshPath);
		}
		else if (type >= OBJECT_ITEM_EQUIP_TOP_BODY)
		{
			FString szMeshPath = "SkeletalMesh'/Game/Cloth/" + szMeshName + "'";
			item.skeletalMesh = LoadSkeletalMeshFromPath(szMeshPath);
		}
		else
		{
			FString szMeshPath = "StaticMesh'/Game/Item/" + szMeshName + "'";
			item.staticmesh = LoadStaticMeshFromPath(szMeshPath);
		}
	}

	item.type = type;
	m_objectItemMap.Add(id, item);
}

ObjectItem* AObjectItemManager::GetObjectItem(FName id)
{
	return m_objectItemMap.Find(id);
}