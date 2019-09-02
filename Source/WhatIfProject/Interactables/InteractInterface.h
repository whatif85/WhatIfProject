// Copyright FastForward Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

//#include "UObject/ObjectMacros.h"
//#include "UObject/ScriptMacros.h"
#include "UObject/Interface.h"

//#include "Characters/Squad/PlayerCharacter.h"

#include "Blueprint/BlueprintSupport.h"

//#include "Interactables/Items/BaseInteractable.h"
#include "Interactables/Items/F_ItemInfo.h"

// Forward Declaration (to avoid circular dependencies)
class ABaseInteractable;
//class APlayerCharacter;

#include "InteractInterface.generated.h"

/* Must have BlueprintType as a specifier to have this interface exposed to blueprints
   with this line you can easily add this interface to any blueprint class */
UINTERFACE(BlueprintType)
class WHATIFPROJECT_API UInteractInterface : public UInterface
{
	//GENERATED_UINTERFACE_BODY() // Legacy
	GENERATED_BODY()
};

class WHATIFPROJECT_API IInteractInterface
{
	//GENERATED_IINTERFACE_BODY() // Legacy
	GENERATED_BODY()

public:
	// Classes using this interface MAY implement RegisterInteractable
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interfaces\|Interactable")
		void RegisterInteractable(ABaseInteractable* Interactable);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interfaces\|Interactable")
		void UnregisterInteractable(ABaseInteractable* Interactable);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interfaces\|Interactable")
		void Interact(APlayerCharacter* PlayerInstigator);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interfaces\|Interactable")
		void PickUp(F_ItemInfo ItemInfo);

	/*
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void I_Converse(F_CreatureInfo CreatureInfo) = 0;
	*/
};
