// Copyright FastForward Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/BlueprintSupport.h"

#include "Interactables/InteractInterface.h"
#include "Interactables/Items/BaseInteractable.h"
#include "Interactables/Items/F_ItemInfo.h"

class APlayerCharacter;

#include "BaseItem.generated.h"

/**
 * 
 */
UCLASS()
class WHATIFPROJECT_API ABaseItem : public ABaseInteractable
{
public:
	GENERATED_BODY()

	ABaseItem(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interfaces\|Interactable")
		F_ItemInfo ItemInfo;

	virtual void Interact(APlayerCharacter PlayerInstigator)/* override*/;
};
