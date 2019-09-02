// Copyright FastForward Studios. All Rights Reserved.

#include "ThirdPersonPlayerGameMode.h"
#include "Characters/Squad/PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"


AThirdPersonPlayerGameMode::AThirdPersonPlayerGameMode() : AGameMode()
{
	// Set default pawn class to our Blueprint character
	static ConstructorHelpers::FClassFinder<APlayerController> ThirdPersonPlayerControllerBPClass(TEXT("/Game/Blueprints/Game/BP_ThirdPerson_PlayerController"));
	if (ThirdPersonPlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = ThirdPersonPlayerControllerBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Characters/Squad/BP_CH_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// TODO: Old code, did not work. Review before deleting
	// Use custom player controller
	//PlayerControllerClass = AThirdPersonPlayerController::StaticClass();

	// Set other default framework classes for this game mode
	//DefaultPawnClass = APlayerCharacter::StaticClass();

	/*GameStateClass = ACustomGameState::StaticClass();
	HUDClass = ACustomGameHUD::StaticClass();
	ReplaySpectatorPlayerControllerClass = ACustomReplaySpectatorPlayerController::StaticClass();
	SpectatorClass = ACustomSpectatorClass::StaticClass();*/
}