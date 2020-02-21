// Copyright FastForward Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Interactables/Items/BaseInteractable.h"
#include "Interactables/Items/F_ItemInfo.h"

#include "Components/TimelineComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Runtime/Engine/Classes/Components/PawnNoiseEmitterComponent.h"

//class ABaseInteractable;
//class ABaseItem;

#include "PlayerCharacter.generated.h"

UCLASS()
class WHATIFPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/**
	 *	PUBLIC VARIABLES
	 */
	// Sets default values for this character's properties
	APlayerCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* FirstPersonArms;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* HandHeldItem;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;

	/* Projectile class to spawn */
	// TODO: This should be a generic projectile superclass
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ABallProjectile> ProjectileClass;

	/** Location on gun mesh where projectiles should spawn. */
	// TODO: This was originally designed for a 1st person gun. In the future, this should be for dark energy casting. Another separate gun-related "muzzle" will be used
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	// TODO: Currently disabled, but will be used to switch between 3rd and 1st person view
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns FirstPersonArms subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return FirstPersonArms; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/// Components
	UPROPERTY(BlueprintReadWrite, NonTransactional, Category = "Components")
		USkeletalMeshComponent* BaseTorso;
	UPROPERTY(BlueprintReadWrite, NonTransactional, Category = "Components")
		USkeletalMeshComponent* Shoulders;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		USkeletalMeshComponent* Waist;
	UPROPERTY(BlueprintReadWrite, NonTransactional, Category = "Components")
		USkeletalMeshComponent* Legs1;


	/// Attributes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Health")
		float CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Health")
		float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Health")
		float HealthPercentage;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Health")
	//	float RegenHealth = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Health")
		bool bRedFlashDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Stamina")
		float CurrentStamina = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Stamina")
		float MaxStamina = 100.0f;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Stamina")
	//	float RegenStamina = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Tech Power")
		float CurrentTechPower = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Tech Power")
		float MaxTechPower = 100.0f;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Shields")
	//	float RegenShields = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Dark Energy")
		float CurrentDarkEnergy = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Dark Energy")
		float MaxDarkEnergy = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Dark Energy")
		float DarkEnergyPercentage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Dark Energy")
		float PreviousDarkEnergy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Dark Energy")
		float DarkEnergyValue; // "mana" cost
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Dark Energy")
		UCurveFloat* DarkEnergyCurve;

	//UPROPERTY(EditAnywhere, Category = "Attributes\|Dark Energy")
	UTimelineComponent* AttributeTimeline;

	UPROPERTY(EditAnywhere, Category = "Attributes\|Dark Energy")
		FTimerHandle MemberTimerHandle; // interval to cast (2 secs)
	UPROPERTY(EditAnywhere, Category = "Attributes\|Dark Energy")
		FTimerHandle DarkEnergyTimerHandle; // cooldown to refill (5 secs, may be variable to decrease cooldown at skill level up)
	UPROPERTY(EditAnywhere, Category = "Attributes\|Dark Energy")
		class UMaterialInterface* DarkEnergyDefaultMaterial;
	UPROPERTY(EditAnywhere, Category = "Attributes\|Dark Energy")
		class UMaterialInterface* DarkEnergyBurnoutMaterial; // TODO: also overheat material
	UPROPERTY(BlueprintReadOnly, Category = "Attributes\|Dark Energy")
		float CurveFloatValue;
	UPROPERTY(BlueprintReadOnly, Category = "Attributes\|Dark Energy")
		float TimelineValue;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Psi")
		float CurrentPsi = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Psi")
		float MaxPsi = 100.0f;

	// TODO: Design 'magic schools': magic = phlebotinum
	// - Tech
	// - Dark Matter
	// - Dark Energy
	// - Psi


	/// Gameplay: Character Status
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Character Status")
		bool bIsActive = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Character Status")
		bool bIsAlive = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Character Status")
		bool bIsSprinting = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Character Status")
		bool bIsAiming;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Character Status")
		bool bIsPaused;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Character Status")
		bool bIsArmed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Character Status")
		bool bHasWeaponHolstered;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Character Status")
		bool bIsAttacking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Character Status")
		bool bIsBusy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Character Status")
		bool bHasBeenDetected;

	/// Gameplay: Upgradable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Upgradable\|Physical")
		float SprintSpeedMultiplier = 1.4f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Upgradable\|Physical")
		float FitnessBonus = 1.0f;

	/// Gameplay: Personality
	// Note: Default values are set midway. Values are set at Character creation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Personality")
		float ProgressiveVsTraditionalist = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Personality")
		float IndividualistVsCollectivist = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Personality")
		float IndependentVsCompliant = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Personality")
		float IdealistVsPragmatist = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Personality")
		float CasualVsProfessional = 0.5f;

	/// Gameplay: Skills
	// TODO: Using hardcoded values, should all be set to false at startup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Athletic")
		bool bClimb = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Athletic")
		bool bFitness = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Athletic")
		bool bTumble = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Armor")
		bool bLightArmor = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Armor")
		bool bMediumArmor = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Armor")
		bool bHeavyArmor = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Charisma")
		bool bAppraise;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Charisma")
		bool bBluff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Charisma")
		bool bPersuasion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Charisma")
		bool bIntimidate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Charisma")
		bool bPerform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Charisma")
		bool bTaunt;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Combat")
		bool bAssaultTraining = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Combat")
		bool bMelee = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Combat")
		bool bMartialArts = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Combat")
		bool bSharpshooter = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Combat")
		bool bWeaponsPistols = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Combat")
		bool bWeaponsShotguns = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Combat")
		bool bWeaponsAssaultRifles = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Combat")
		bool bWeaponsSniperRifles = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Combat")
		bool bWeaponsGrenades = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Dark Energy")
		bool bDarkEnergyUser; // TODO: testing https://www.youtube.com/watch?v=Nt4W1B8cKy8 should be set to false

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Dark Energy")
		bool bConcentrate = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Investigation")
		bool bSearch = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Investigation")
		bool bSpot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Investigation")
		bool bIntuition = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Investigation")
		bool bListen;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Medic")
		bool bFirstAid = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Medic")
		bool bMedicine = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Rogue")
		bool bStealthMovement = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Rogue")
		bool bBlendInShadows = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Tech Power")
		bool bBarrier = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Tech Power")
		bool bDisableDevices = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Tech Power")
		bool bProgramming = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Tech Power")
		bool bDecrypt = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Tech Power")
		bool bHack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Tech Power")
		bool bCrafting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Tech Power")
		bool bElectronics = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Tech Power")
		bool bRobotics = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Tech Power")
		bool bChemistry = false;


	/// Gameplay: Leveling
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Leveling")
		int PlayerLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Leveling")
		int PlayerSkillPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Leveling")
		float PlayerCurrentXP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Leveling")
		float PlayerNeededXP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Leveling")
		FName PlayerClassName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Leveling")
		FName PlayerCharacterFirstName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Leveling")
		FName PlayerCharacterCallsign;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Leveling")
		FName PlayerCharacterLastName;

	/// Gameplay: Interactable
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay\|Interactable")
		TArray<ABaseInteractable*> Interactables;


	/// Gameplay: Inventory And Equipment
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay\|InventoryAndEquipment")
		TArray<F_ItemInfo> Inventory;


	/// Gameplay: Perception AI
	/*A Pawn Noise Emitter component which is used in order to emit the sounds to nearby AIs*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gameplay\|Perception AI")
		UPawnNoiseEmitterComponent* PawnNoiseEmitterComp;

	/**
	 *	PUBLIC FUNCTIONS
	 */
	/// Gameplay: Leveling
	UFUNCTION(BlueprintCallable, Category = "Gameplay\|Leveling")
		void AddXP(float AddedXP);

	UFUNCTION(BlueprintCallable, Category = "Gameplay\|Leveling")
		void AddSkillPoints();


	/// Gameplay: Interactable
	UFUNCTION(BlueprintCallable, Category = "Gameplay\|Interactable")
		virtual void HandleRegisterInteractable(ABaseInteractable* Interactable);

	UFUNCTION(BlueprintCallable, Category = "Gameplay\|Interactable")
		virtual void HandleUnregisterInteractable(ABaseInteractable* Interactable);

	UFUNCTION(BlueprintCallable, Category = "Gameplay\|Interactable")
		virtual void UpdateNearestInteractable();

	UFUNCTION(BlueprintCallable, Category = "Gameplay\|Interactable")
		virtual void GetNearestInteractable();

	UFUNCTION(BlueprintCallable, Category = "Gameplay\|Interactable")
		virtual void SetNearestInteractable(ABaseInteractable* Interactable);


	/// Gameplay: Inventory And Equipment
	// TODO: Confirm if this function does require const at the end as a "pure" function
	UFUNCTION(BlueprintCallable, Category = "Gameplay\|Inventory And Equipment")
		virtual bool HasSpaceInInventory() const;


	/// Interfaces: Interactable
	UFUNCTION(BlueprintCallable, Category = "Interfaces\|Interactable")
		virtual void RegisterInteractable(ABaseInteractable* Interactable);

	UFUNCTION(BlueprintCallable, Category = "Interfaces\|Interactable")
		virtual void UnregisterInteractable(ABaseInteractable* Interactable);

	UFUNCTION(BlueprintCallable, Category = "Interfaces\|Interactable")
		virtual void PickUp(F_ItemInfo ItemInfo);


	/** Check current health percentage in main status UI */
	UFUNCTION(BlueprintPure, Category = "Attributes\|Health")
		float GetHealth();

	/** Return health percentage for the UI to read */
	UFUNCTION(BlueprintPure, Category = "Attributes\|Health")
		FText GetHealthIntText();

	/** Temporary invincibility before next damage */
	UFUNCTION()
		void SetDamageTimer();

	/** Set damageable character after a short invincibility break */
	UFUNCTION()
		void SetDamageState();

	UFUNCTION(BlueprintPure, Category = "Attributes\|Health")
		bool PlayRedFlashDamage();

	//UFUNCTION(BlueprintCallable, Category = "Attributes\|Health")
	/*	void ReceivePointDamage(
			float Damage, const UDamageType* DamageType,
			FVector HitLocation, FVector HitNormal, UPrimitiveComponent* HitComponent,
			FName BoneName, FVector ShotFromDirection,
			AController* InstigatedBy, AActor* DamageCauser, const FHitResult& HitInfo);*/

	UFUNCTION(BlueprintCallable, Category = "Attributes\|Health")
		void UpdateHealth(float HealthChange);


	/** Check current DE percentage in main status UI */
	UFUNCTION(BlueprintPure, Category = "Attributes\|Dark Energy")
		float GetDarkEnergy();

	/** Return dark energy percentage for the UI to read */
	UFUNCTION(BlueprintPure, Category = "Attributes\|Dark Energy")
		FText GetDarkEnergyIntText();

	UFUNCTION()
		void SetDarkEnergyValue();

	UFUNCTION()
		void SetDarkEnergyState();

	UFUNCTION()
		void SetDarkEnergyChange(float DarkEnergyChange);

	UFUNCTION()
		void UpdateDarkEnergy();


	/// Perception AI
	UFUNCTION(BlueprintCallable, Category = AI)
		void ReportNoise(USoundBase* SoundToPlay, float Volume);

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;


protected:
	/**
	*	PROTECTED FUNCTIONS
	*/
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	/// Input: Mouse
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

	/// Input: VR
	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/// Input: Gamepad Rotation
	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);
	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	/// Input: Movement
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

	/// Gameplay
	// Delegate function
	UFUNCTION(BlueprintCallable, Category = "Attributes\|Health")
		virtual float TakeDamage
		(
			float DamageAmount,
			struct FDamageEvent const& DamageEvent,
			class AController* EventInstigator,
			AActor* DamageCauser
		);

	UFUNCTION(BlueprintCallable, Category = "Attributes\|Dark Energy")
		void OnDarkEnergyCast();
};
