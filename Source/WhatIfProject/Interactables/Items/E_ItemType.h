// Copyright FastForward Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "E_ItemType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum E_ItemType
{
	ITEM_Default		UMETA(DisplayName = "None"),
	ITEM_Credits		UMETA(DisplayName = "Credits"),
	ITEM_Clothes		UMETA(DisplayName = "Clothes"),
	ITEM_Armor			UMETA(DisplayName = "Armor"),
	ITEM_Implants		UMETA(DisplayName = "Implants"),
	ITEM_Weapon			UMETA(DisplayName = "Weapon"),
	ITEM_Electronics	UMETA(DisplayName = "Electronics"),
	ITEM_Edible			UMETA(DisplayName = "Edible"),
	ITEM_W_Interact		UMETA(DisplayName = "World Interactable"),
	ITEM_Misc			UMETA(DisplayName = "Miscellaneous")
};
