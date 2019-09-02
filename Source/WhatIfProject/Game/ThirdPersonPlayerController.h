// Copyright FastForward Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ThirdPersonPlayerController.generated.h"


UCLASS(Blueprintable, BlueprintType)
class WHATIFPROJECT_API AThirdPersonPlayerController : public APlayerController
{
	GENERATED_BODY()

		AThirdPersonPlayerController();

public:
	// Reference UMG Assets in the Editor
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> InGameMenu_Reference;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> ThirdPersonGameHUD_Reference;

	// Variables to hold the widgets
	UUserWidget* InGameMenu;
	UUserWidget* ThirdPersonGameHUD;

	// Override BeginPlay()
	virtual void BeginPlay() override;

	void SetupInput();
	void OpenInGameMenu();
};
