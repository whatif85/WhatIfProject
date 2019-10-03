// Copyright FastForward Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include <UMG.h>

#include "ConsoleGameMenu.generated.h"


/**
 * 
 */
UCLASS()
class WHATIFPROJECT_API UConsoleGameMenu : public UUserWidget
{
public:
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Resources\|Credits")
	UTextBlock* CreditsNumberLabel = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Resources\|Credits")
	int32 CreditsNumber = 1500;	
};
