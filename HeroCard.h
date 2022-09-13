#pragma once

#include "CoreMinimal.h"
#include "HeroAgent.h"

UENUM(BlueprintType)
enum class HERO_CARD_STATE : uint8
{
	UNENABLE = 0,
	ENABLED,
	SELECTED,
	DISABLED
};
 
struct HeroCard
{
	HeroCard()
	{ 
		state = HERO_CARD_STATE::UNENABLE;
		pHero = nullptr;
	}

	void SetRect(int _x, int _y, int _width, int _height)
	{
		x = _x;
		y = _y;
		width = _width;
		height = _height;
	}

	int x, y;
	int width, height;
	HERO_CARD_STATE state;
	AHeroAgent* pHero;
 };
 
struct HeroCardData
{
	HeroCardData()
	{
		index = -1;
	}

	AHeroAgent* GetHero(int _index)
	{
		return heroCardsList[_index].pHero;
	}

	HERO_CARD_STATE GetState(int _index)
	{
		return heroCardsList[_index].state;
	}

	void SetState(int _index, HERO_CARD_STATE state)
	{
		heroCardsList[_index].state = state;
	}

	void ClearSelect()
	{
		for (int i = 0; i < heroCardsList.Num(); i++)
		{
			if (heroCardsList[i].state == HERO_CARD_STATE::UNENABLE || heroCardsList[i].state == HERO_CARD_STATE::DISABLED) {}
			else heroCardsList[i].state = HERO_CARD_STATE::ENABLED;
		}
	}

	bool Select(int _index)
	{
		if (heroCardsList[_index].state == HERO_CARD_STATE::UNENABLE || heroCardsList[_index].state == HERO_CARD_STATE::DISABLED)
			return false;
		else
		{
			for (int i = 0; i < heroCardsList.Num(); i++)
			{
				if (heroCardsList[i].state == HERO_CARD_STATE::UNENABLE || heroCardsList[i].state == HERO_CARD_STATE::DISABLED) {}
				else heroCardsList[i].state = (i == _index) ? HERO_CARD_STATE::SELECTED : HERO_CARD_STATE::ENABLED;
			}

			return true;
		}
	}

	void Select(AHeroAgent* _pHero)
	{
		for (int i = 0; i < heroCardsList.Num(); i++)
		{
			if (heroCardsList[i].pHero == _pHero)
			{
				heroCardsList[i].state = HERO_CARD_STATE::SELECTED;
				heroCardsList[i].pHero->SetSelectedState(true);
				break;
			}
		}
	}

	void Enable(int _index, AHeroAgent* _pHero)
	{
		heroCardsList[_index].state = HERO_CARD_STATE::ENABLED;
		heroCardsList[_index].pHero = _pHero;
	}

	void Disable(AHeroAgent* _pHero)
	{
		if (_pHero && _pHero->GetGroupType() == AGENT_GROUP_TYPE::PLAYER)
		{
			for (int i = 0; i < heroCardsList.Num(); i++)
			{
				if (heroCardsList[i].pHero == _pHero)
				{
					heroCardsList[i].state = HERO_CARD_STATE::DISABLED;
					break;
				}
			}
		}
	}

	int index;
	TArray<HeroCard> heroCardsList;
};