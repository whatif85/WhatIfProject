// Copyright FastForward Games. All Rights Reserved.

#include "PlayerCharacter.h"

#include "HeadMountedDisplayFunctionLibrary.h"

#include "Camera/CameraComponent.h"
#include "Classes/Components/SkeletalMeshComponent.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/DamageType.h"

#include "Projectiles/Weapons/BallProjectile.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"

//#include "MotionControllerComponent.h" // VR control
//#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId

#include "Interactables/F_InteractableVariables.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	PawnNoiseEmitterComp = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitterComp"));

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera (default camera set before any other camera)
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a First Person CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Add Skeletal Mesh Sockets
	BaseTorso = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Base Torso"));
	Shoulders = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shoulders"));
	Waist = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Waist"));
	Legs1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs1"));

	//XP_Label = nullptr;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	// TODO: When working on 1st person view, there should be a "headless" (invisible head) body-mesh, which should look complete when using mirrors
	/*FirstPersonArms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	FirstPersonArms->SetOnlyOwnerSee(true);
	FirstPersonArms->SetupAttachment(FirstPersonCameraComponent);
	FirstPersonArms->bCastDynamicShadow = false;
	FirstPersonArms->CastShadow = false;
	FirstPersonArms->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	FirstPersonArms->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);*/

	// Create a hand-held mesh component
	// TODO: This should only work with equipped handheld items
	HandHeldItem = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_HandHeldItem;"));
	HandHeldItem->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	HandHeldItem->bCastDynamicShadow = false;
	HandHeldItem->CastShadow = false;
	HandHeldItem->SetupAttachment(FirstPersonArms, TEXT("GripPoint"));
	HandHeldItem->SetupAttachment(RootComponent);

	/*FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_HandHeldItem);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));*/

	// Default offset from the character location for projectiles to spawn
	//GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_HandHeldItem 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Player's starting conditions
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	HealthPercentage = 1.0f; // 100%
	bCanBeDamaged = true;

	CurrentStamina = 100.0f;
	CurrentTechPower = 100.0f;
	
	MaxDarkEnergy = 100.0f;
	CurrentDarkEnergy = MaxDarkEnergy;
	DarkEnergyPercentage = 1.0f;
	PreviousDarkEnergy = DarkEnergyPercentage;
	DarkEnergyValue = 0.0f; // not casting any phlebotinum charges
	bDarkEnergyUser = true; // TODO: default protagonist can use dark energy fields. Will be removed when designing RPG character creation

	// TODO: this will be moved to a separate function that will trigger only when this character can use dark energy skills for the 1st time
	if (DarkEnergyCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineCallbackEnd;

		TimelineCallback.BindUFunction(this, FName("SetDarkEnergyValue"));
		TimelineCallbackEnd.BindUFunction(this, FName("SetDarkEnergyState"));

		AttributeTimeline = NewObject<UTimelineComponent>(this, FName("Dark Energy Animation"));
		AttributeTimeline->AddInterpFloat(DarkEnergyCurve, TimelineCallback);
		AttributeTimeline->SetTimelineFinishedFunc(TimelineCallbackEnd);
		AttributeTimeline->RegisterComponent();

		//AttributeTimeline.AddInterpFloat(DarkEnergyCurve, TimelineCallback);
		//AttributeTimeline.SetTimelineFinishedFunc(TimelineCallbackEnd);
	}

	PlayerLevel = 1;
	PlayerSkillPoints = 3;
	PlayerCurrentXP = 0.0f;
	PlayerNeededXP = 100.0f;

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	//FP_HandHeldItem;->AttachToComponent(FirstPersonArms, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//AttributeTimeline->TickTimeline(DeltaTime);
	if (AttributeTimeline != nullptr)
	{
		AttributeTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	}

	// TODO: Move elsewhere other than Tick whenever possible
	UpdateNearestInteractable();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Walk", IE_Pressed, this, &APlayerCharacter::ToggleWalk);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::ToggleCrouch);

	/*
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::StopSprinting);
	*/

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::OnDarkEnergyCast);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &APlayerCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &APlayerCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &APlayerCharacter::OnResetVR);
}

void APlayerCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void APlayerCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void APlayerCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::ToggleWalk()
{

}

void APlayerCharacter::ToggleCrouch()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}
	else
	{
		Crouch();
		GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	}
}

/*
void APlayerCharacter::Sprint()
{
	bIsSprinting = true;

	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}

	GetCharacterMovement()->MaxWalkSpeed *= SprintSpeedMultiplier;

	// TODO: Set Timer By Event
	//GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &APlayerCharacter::DecreaseStamina, 1.0f, true, 2.0f);
	// TODO: Clear and Invalidate Timer By Handle (Stop decreasing stamina once at 0)
}

void APlayerCharacter::StopSprinting()
{
	bIsSprinting = false;

	GetCharacterMovement()->MaxWalkSpeed /= SprintSpeedMultiplier;
}


void APlayerCharacter::IncreaseStamina()
{
	if (CurrentStamina >= MaxStamina)
	{
		// TODO: Clear and Invalidate Timer By Handle (Stop increasing stamina once at 100)
	}
	else
	{
		CurrentStamina += RegenStamina;
	}
}

void APlayerCharacter::DecreaseStamina()
{
	if (CurrentStamina <= 0.0f)
	{
		bIsSprinting = false;

		GetCharacterMovement()->MaxWalkSpeed /= SprintSpeedMultiplier;
	}
	else
	{
		CurrentStamina -= FitnessBonus;
	}
}
*/


/**
*	LEVELING
*/
void APlayerCharacter::AddXP(float AddedXP)
{
	PlayerCurrentXP += AddedXP;

	if (PlayerCurrentXP >= PlayerNeededXP)
	{
		PlayerCurrentXP -= PlayerNeededXP;
		PlayerLevel++;

		AddSkillPoints();

		PlayerNeededXP *= 2.0f;
	}
}

void APlayerCharacter::AddSkillPoints()
{
	PlayerSkillPoints += 3;
}


/**
*	INTERACTABLES
*/
void APlayerCharacter::RegisterInteractable(ABaseInteractable* Interactable)
{
	HandleRegisterInteractable(Interactable);
}

void APlayerCharacter::UnregisterInteractable(ABaseInteractable* Interactable)
{
	HandleUnregisterInteractable(Interactable);
}

void APlayerCharacter::PickUp(F_ItemInfo ItemInfo)
{
	
}

void APlayerCharacter::HandleRegisterInteractable(ABaseInteractable* Interactable)
{
	// Add unique instance of this interactable
	Interactables.AddUnique(Interactable);
}

void APlayerCharacter::HandleUnregisterInteractable(ABaseInteractable* Interactable)
{
	Interactables.Remove(Interactable);
}

void APlayerCharacter::UpdateNearestInteractable()
{
	// If there's anything (other than nothing)
	if (Interactables.Num() > 0)
	{
		GetNearestInteractable();
	}
	else
	{
		// If no interactable is found set it to nothing
		SetNearestInteractable((ABaseInteractable*) nullptr);
	}		
}

void APlayerCharacter::GetNearestInteractable()
{
	//UPrimitiveComponent* InteractablePrimitive;
	F_InteractableVariables InteractableVariables;

	InteractableVariables.DotProduct = -1; // Lowest value for dot product
}

void APlayerCharacter::SetNearestInteractable(ABaseInteractable* Interactable)
{

}


/**
*	INVENTORY AND EQUIPMENT
*/
bool APlayerCharacter::HasSpaceInInventory() const
{
	bool bReturnValue{};
	int32 CallFunc_Array_Length_bReturnValue{};
	bool CallFunc_Less_IntInt_bReturnValue{};

	return bReturnValue;
}


/**
*	ATTRIBUTES
*/
float APlayerCharacter::TakeDamage(
	float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser
)
{
	bCanBeDamaged = false;
	bRedFlashDamage = true;

	UpdateHealth(-DamageAmount);
	SetDamageTimer();

	return DamageAmount;
}

void APlayerCharacter::OnDarkEnergyCast()
{
	// try and fire a projectile (BP class referenced in Blueprint)
	// Must: find projectile class, have enough dark energy available to cast, be a dark energy user
	if (ProjectileClass != NULL &&
		FMath::IsNearlyZero(CurrentDarkEnergy, 0.001f) == false &&
		bDarkEnergyUser == true)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			const FRotator SpawnRotation = GetControlRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);
				
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<ABallProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}

		// Stop timeline to prevent further dark energy loss
		AttributeTimeline->Stop();
		GetWorldTimerManager().ClearTimer(DarkEnergyTimerHandle);

		// Decrease available DE and restore castable dark energy after cooldown
		SetDarkEnergyChange(-20.0f); // TODO: -20.0f should be a variable that decreases depending on DE 'spell' power
		// TODO: 5.0f should be a variable. Cooldown depends on how powerful the spell was
		GetWorldTimerManager().SetTimer(DarkEnergyTimerHandle, this, &APlayerCharacter::UpdateDarkEnergy, 5.0f, false); // false: do not loop
	}
}

float APlayerCharacter::GetHealth()
{
	return HealthPercentage;
}

FText APlayerCharacter::GetHealthIntText()
{
	int32 HitPoints = FMath::RoundHalfFromZero(HealthPercentage * 100);
	FString HP_String = FString::FromInt(HitPoints);
	FString Health_HUD = HP_String + FString(TEXT("%"));
	FText HP_Text = FText::FromString(Health_HUD);

	return HP_Text;
}

void APlayerCharacter::SetDamageTimer()
{
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &APlayerCharacter::SetDamageState, 2.0f, false);
}

void APlayerCharacter::SetDamageState()
{
	bCanBeDamaged = true;
}

bool APlayerCharacter::PlayRedFlashDamage()
{
	if (bRedFlashDamage == true)
	{
		// only play once
		bRedFlashDamage = false;

		return true;
	}

	return false;
}

//void APlayerCharacter::ReceivePointDamage(
//	float Damage, const UDamageType* DamageType,
//	FVector HitLocation, FVector HitNormal, UPrimitiveComponent* HitComponent,
//	FName BoneName, FVector ShotFromDirection,
//	AController* InstigatedBy, AActor* DamageCauser, const FHitResult& HitInfo)
//{
//	bCanBeDamaged = false;
//	bRedFlashDamage = true;
//
//	UpdateHealth(-Damage);
//	SetDamageTimer();
//}

void APlayerCharacter::UpdateHealth(float HealthChange)
{
	CurrentHealth += HealthChange;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);

	HealthPercentage = CurrentHealth / MaxHealth;
}

float APlayerCharacter::GetDarkEnergy()
{
	return DarkEnergyPercentage;
}

FText APlayerCharacter::GetDarkEnergyIntText()
{
	int32 DarkEnergyPoints = FMath::RoundHalfFromZero(DarkEnergyPercentage * 100);
	FString DE_String = FString::FromInt(DarkEnergyPoints);
	FString Max_DE_String = FString::FromInt(MaxDarkEnergy);
	FString DarkEnergy_HUD = DE_String + FString(TEXT("/") + Max_DE_String);
	FText DE_Text = FText::FromString(DarkEnergy_HUD);

	return DE_Text;
}

void APlayerCharacter::SetDarkEnergyValue()
{
	TimelineValue = AttributeTimeline->GetPlaybackPosition();

	CurveFloatValue = PreviousDarkEnergy + DarkEnergyValue * DarkEnergyCurve->GetFloatValue(TimelineValue);

	// Prevent value to go less than 0 or above 1
	CurrentDarkEnergy = FMath::Clamp(CurveFloatValue * MaxHealth, 0.0f, MaxDarkEnergy);
	DarkEnergyPercentage = FMath::Clamp(CurveFloatValue, 0.0f, 1.0f);
}

void APlayerCharacter::SetDarkEnergyState()
{
	// TODO: This variable should be changed to a temporary state rather than merely be a DE user
	bDarkEnergyUser = true;

	DarkEnergyValue = 0.0f;

	if (DarkEnergyDefaultMaterial)
	{
		HandHeldItem->SetMaterial(0, DarkEnergyDefaultMaterial);
	}
}

void APlayerCharacter::SetDarkEnergyChange(float DarkEnergyCost)
{
	// TODO: This variable should be changed to a temporary state rather than merely be a DE user
	bDarkEnergyUser = false;
	PreviousDarkEnergy = DarkEnergyPercentage;
	DarkEnergyValue = (DarkEnergyCost / MaxDarkEnergy);

	if (DarkEnergyBurnoutMaterial)
	{
		HandHeldItem->SetMaterial(0, DarkEnergyBurnoutMaterial);
	}

	if (AttributeTimeline != nullptr)
	{
		AttributeTimeline->PlayFromStart();
	}
}

void APlayerCharacter::UpdateDarkEnergy()
{
	PreviousDarkEnergy = DarkEnergyPercentage;
	DarkEnergyPercentage = CurrentDarkEnergy / MaxDarkEnergy;
	DarkEnergyValue = 1.0f;

	if (AttributeTimeline != nullptr)
	{
		AttributeTimeline->PlayFromStart();
	}
}

void APlayerCharacter::ReportNoise(USoundBase* SoundToPlay, float Volume)
{
	// If we have a valid sound to play, play the sound and report it to our game
	if (SoundToPlay)
	{
		//Play the actual sound
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundToPlay, GetActorLocation(), Volume);

		//Report that we've played a sound with a certain volume in a specific location
		MakeNoise(Volume, this, GetActorLocation());
	}

}