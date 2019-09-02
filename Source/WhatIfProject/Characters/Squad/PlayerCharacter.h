// Copyright FastForward Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Interactables/Items/BaseInteractable.h"
#include "Interactables/Items/F_ItemInfo.h"

//class ABaseInteractable;
//class ABaseItem;

#include "PlayerCharacter.generated.h"

UCLASS()
class WHATIFPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/**
	 *	VARIABLES
	 */
	/// Components
	UPROPERTY(BlueprintReadWrite, NonTransactional, meta = (Category = "PlayerCharacter", OverrideNativeName = "Base Torso"))
		USkeletalMeshComponent* BaseTorso;
	UPROPERTY(BlueprintReadWrite, NonTransactional, meta = (Category = "PlayerCharacter", OverrideNativeName = "Shoulders"))
		USkeletalMeshComponent* Shoulders;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "PlayerCharacter", OverrideNativeName = "Waist"))
		USkeletalMeshComponent* Waist;
	UPROPERTY(BlueprintReadWrite, NonTransactional, meta = (Category = "PlayerCharacter", OverrideNativeName = "Legs1"))
		USkeletalMeshComponent* Legs1;

	/// Interactable
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay\|Interactable\|Player")
		TArray<ABaseInteractable*> Interactables;

	/// Inventory And Equipment
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay\|InventoryAndEquipment\|Player")
		TArray<F_ItemInfo> Inventory;


	/**
	*	FUNCTIONS
	*/
	/// Interfaces
	UFUNCTION(BlueprintCallable, Category = "Interfaces\|Interactable")
		virtual void RegisterInteractable(ABaseInteractable* Interactable);
	UFUNCTION(BlueprintCallable, Category = "Interfaces\|Interactable")
		virtual void UnregisterInteractable(ABaseInteractable* Interactable);
	UFUNCTION(BlueprintCallable, Category = "Interfaces\|Interactable")
		virtual void PickUp(F_ItemInfo ItemInfo);

	/// Leveling
	UFUNCTION(BlueprintCallable, Category = "Gameplay\|Leveling\|Player")
		void AddXP(float AddedXP);
	UFUNCTION(BlueprintCallable, Category = "Gameplay\|Leveling\|Player")
		void AddSkillPoints();

	/// Interactable
	UFUNCTION(BlueprintCallable, Category = "Gameplay\|Interactable\|Player")
		virtual void HandleRegisterInteractable(ABaseInteractable* Interactable);
	UFUNCTION(BlueprintCallable, Category = "Gameplay\|Interactable\|Player")
		virtual void HandleUnregisterInteractable(ABaseInteractable* Interactable);
	UFUNCTION(BlueprintCallable, Category = "Gameplay\|Interactable\|Player")
		virtual void UpdateNearestInteractable();
	UFUNCTION(BlueprintCallable, Category = "Gameplay\|Interactable\|Player")
		virtual void GetNearestInteractable();
	UFUNCTION(BlueprintCallable, Category = "Gameplay\|Interactable\|Player")
		virtual void SetNearestInteractable(ABaseInteractable* Interactable);

	/// Inventory And Equipment
	// TODO: Confirm if this function does require const at the end as a "pure" function
	UFUNCTION(BlueprintCallable, Category = "Gameplay\|InventoryAndEquipment\|Player")
		virtual bool HasSpaceInInventory() const;


private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;


protected:
	/**
	*	VARIABLES
	*/
	/// Attributes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Health")
		float CurrentHealth = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Health")
		float MaxHealth = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Health")
		float RegenHealth = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Stamina")
		float CurrentStamina = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Stamina")
		float MaxStamina = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Stamina")
		float RegenStamina = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Shields")
		float CurrentShields = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Shields")
		float MaxShields = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Shields")
		float RegenShields = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Dark Energy")
		float CurrentDarkEnergy = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Dark Energy")
		float MaxDarkEnergy = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Dark Energy")
		float RegenDarkEnergy = 1.0f;

	/// Upgradable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgradable\|Physical")
		float SprintSpeedMultiplier = 1.4f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgradable\|Physical")
		float FitnessBonus = 1.0f;

	/// Gameplay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Conditions")
		bool bIsSprinting = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Conditions")
		bool bIsAiming;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Conditions")
		bool bIsAlive = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Conditions")
		bool bIsPaused;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Conditions")
		bool bIsArmed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Conditions")
		bool bHasWeaponHolstered;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Conditions")
		bool bIsAttacking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Conditions")
		bool bIsBusy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Conditions")
		bool bHasBeenDetected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Leveling\|Player")
		int PlayerLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Leveling\|Player")
		int PlayerSkillPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Leveling\|Player")
		float PlayerCurrentXP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Leveling\|Player")
		float PlayerNeededXP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Leveling\|Player")
		FName PlayerClassName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Leveling\|Player")
		FName PlayerCharacterFirstName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Leveling\|Player")
		FName PlayerCharacterCallsign;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Leveling\|Player")
		FName PlayerCharacterLastName;


	/**
	*	FUNCTIONS
	*/
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	/**
	* VR INPUT
	*/
	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/**
	* GAMEPAD ROTATION
	*/
	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);
	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	/**
	* MOUSE INPUT
	*/
	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);
	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/**
	* MOVEMENT INPUT
	*/
	/** Called for forwards/backward input */
	void MoveForward(float Value);
	/** Called for side to side input */
	void MoveRight(float Value);

	void ToggleWalk();
	void ToggleCrouch();
	/*
	void Sprint();
	void StopSprinting();
	*/

	/*
	UFUNCTION(BlueprintNativeEvent)
		void IncreaseStamina();
	UFUNCTION(BlueprintNativeEvent)
		void DecreaseStamina();
	*/
};
