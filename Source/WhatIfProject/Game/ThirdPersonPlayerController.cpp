// Copyright FastForward Studios. All Rights Reserved.

#include "ThirdPersonPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "EngineMinimal.h"


AThirdPersonPlayerController::AThirdPersonPlayerController() : APlayerController()
{

}

void AThirdPersonPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	
	// Check if the asset is assigned in the blueprint
	if (ThirdPersonGameHUD_Reference)
	{
		// Create the widget and store it (same as promoting reference variable in BP editor)
		ThirdPersonGameHUD = CreateWidget<UUserWidget>(this, ThirdPersonGameHUD_Reference);

		// Use the widget directly since there is a reference to it (extra check to make sure the pointer holds the widget)
		if (ThirdPersonGameHUD) { ThirdPersonGameHUD->AddToViewport(); }

		bShowMouseCursor = false;
	}
	
}

// TODO: WIP, doesn't show up
void AThirdPersonPlayerController::SetupInput()
{
	/*
	EnableInput(this);
	check(InputComponent);

	// Delegate key binding to method below
	InputComponent->BindKey(EKeys::Escape, IE_Pressed, this, &AThirdPersonPlayerController::OpenInGameMenu);
	//InputComponent->BindKey("InGameMenu_Main", IE_Pressed, this, &AThirdPersonPlayerController::OpenInGameMenu);
	*/
}

void AThirdPersonPlayerController::OpenInGameMenu()
{
	
	if (InGameMenu_Reference)
	{
		InGameMenu = CreateWidget<UUserWidget>(this, InGameMenu_Reference);

		if (InGameMenu) { InGameMenu->AddToViewport(); }

		bShowMouseCursor = true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't display Menu"));
	}
	
}