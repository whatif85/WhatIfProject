// Copyright FastForward Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ThirdPersonGame_HUD.generated.h"


/**
 * 
 */
UCLASS()
class WHATIFPROJECT_API AThirdPersonGame_HUD : public AHUD
{
	GENERATED_BODY()

public:
	AThirdPersonGame_HUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTexture;

	UPROPERTY(EditAnywhere, Category = "HUD")
		TSubclassOf<class UUserWidget> RHUD_WidgetClass;

	UPROPERTY(EditAnywhere, Category = "HUD")
		class UUserWidget* RCurrentWidget;
};
