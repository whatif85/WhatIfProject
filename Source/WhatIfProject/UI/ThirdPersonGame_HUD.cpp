// Copyright FastForward Studios. All Rights Reserved.


#include "ThirdPersonGame_HUD.h"

#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"

#include "Runtime/UMG/Public/UMG.h"
//#include "Runtime/UMG/Public/UMGStyle.h"
//#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
//#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"


AThirdPersonGame_HUD::AThirdPersonGame_HUD()
{
	// Set the crosshair texture
	// TODO: Texture required
	/*
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTexture = CrosshairTexObj.Object;
	*/

	static ConstructorHelpers::FClassFinder<UUserWidget> HealthBarTexObj(TEXT("/Game/Blueprints/UI/BP_ThirdPersonGame_HUD"));
	RHUD_WidgetClass = HealthBarTexObj.Class;
}

void AThirdPersonGame_HUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition((Center.X), (Center.Y + 20.0f));

	// draw the crosshair
	FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);
}

void AThirdPersonGame_HUD::BeginPlay()
{
	Super::BeginPlay();

	if (RHUD_WidgetClass != nullptr)
	{
		RCurrentWidget = CreateWidget<UUserWidget>(GetWorld(), RHUD_WidgetClass);

		if (RCurrentWidget != nullptr)
		{
			RCurrentWidget->AddToViewport();
		}
	}
}