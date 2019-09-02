// Copyright FastForward Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
class ABaseInteractable;
#include "F_InteractableVariables.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct F_InteractableVariables
{
public:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DotProduct;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		ABaseInteractable* Interactable;
};
