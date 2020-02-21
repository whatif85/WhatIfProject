// Copyright FastForward Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "E_AI_SquadState.generated.h"


UENUM(BlueprintType)
enum E_AI_SquadState
{
	AI_STATE_Default				UMETA(DisplayName = "AI State Default"),		// Default
	AI_STATE_Roam					UMETA(DisplayName = "AI State Roam"),			// Roam
	AI_STATE_FollowPlayer			UMETA(DisplayName = "AI State Follow Player")	// Follow Player
};
