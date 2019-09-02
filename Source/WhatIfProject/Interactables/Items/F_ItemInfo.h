// Copyright FastForward Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "E_ItemType.h"
#include "E_SlotType.h"
#include "F_ItemInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct F_ItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<E_ItemType> ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UClass* ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ItemIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<E_SlotType> ItemSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ItemValue;
};
