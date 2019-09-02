// Copyright FastForward Games. All Rights Reserved.

#include "PlayerCharacter.h"

#include "HeadMountedDisplayFunctionLibrary.h"

#include "Camera/CameraComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

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

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Add Skeletal Mesh Sockets
	BaseTorso = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Base Torso"));
	Shoulders = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shoulders"));
	Waist = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Waist"));
	Legs1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs1"));

	// Player's Starting Conditions
	CurrentHealth = 100.0f;
	CurrentStamina = 100.0f;
	CurrentShields = 100.0f;
	CurrentDarkEnergy = 100.0f;

	PlayerLevel = 1;
	PlayerSkillPoints = 3;
	PlayerCurrentXP = 0.0f;
	PlayerNeededXP = 100.0f;

	//XP_Label = nullptr;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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