// Copyright FastForward Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Interactables/Items/F_ItemInfo.h"
#include "Characters/AI/E_AI_SquadState.h"

#include "Components/TimelineComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BehaviorTree.h"

#include "Squad_Amy.generated.h"

UCLASS()
class WHATIFPROJECT_API ASquad_Amy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASquad_Amy();

	/// Pawn Sensing
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UPawnNoiseEmitterComponent* PawnNoiseEmitterComp;

	UPROPERTY(VisibleAnywhere)
		UPawnSensingComponent* PawnSensingComp;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TEnumAsByte<E_AI_SquadState> AIState;

	UPROPERTY(EditDefaultsOnly)
		UBehaviorTree* BehaviorTree;

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
		float CurrentTech = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Tech Power")
		float MaxTech = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Tech Power")
		float TechPercentage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Tech Power")
		float PreviousTech;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Tech Power")
		float TechValue; // "mana" cost
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes\|Tech Power")
		UCurveFloat* TechCurve;

	//UPROPERTY(EditAnywhere, Category = "Attributes\|Tech Power")
	UTimelineComponent* AttributeTimeline;

	UPROPERTY(EditAnywhere, Category = "Attributes\|Tech Power")
		FTimerHandle MemberTimerHandle; // interval to cast (2 secs)
	UPROPERTY(EditAnywhere, Category = "Attributes\|Tech Power")
		FTimerHandle TechTimerHandle; // cooldown to refill (5 secs, may be variable to decrease cooldown at skill level up)
	UPROPERTY(EditAnywhere, Category = "Attributes\|Tech Power")
		class UMaterialInterface* TechDefaultMaterial;
	UPROPERTY(EditAnywhere, Category = "Attributes\|Tech Power")
		class UMaterialInterface* TechBurnoutMaterial; // TODO: also overheat material
	UPROPERTY(BlueprintReadOnly, Category = "Attributes\|Tech Power")
		float CurveFloatValue;
	UPROPERTY(BlueprintReadOnly, Category = "Attributes\|Tech Power")
		float TimelineValue;

	/// Gameplay: Character Status
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Character Status")
		bool bIsActive = false;
	// Squad members are 'dead' when down. Only the player character can die
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
	// Squad members are detected upon player being spotted

	/// Gameplay: Upgradable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Upgradable\|Physical")
		float SprintSpeedMultiplier = 1.4f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Upgradable\|Physical")
		float FitnessBonus = 1.0f;

	/// Gameplay: Personality
	// Squad characters develop their own personality and cannot be changed by the player
	// Ideally, these value may change depending on how they develop according to player's decisions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Personality")
		float ProgressiveVsTraditionalist = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Personality")
		float IndividualistVsCollectivist = 0.3f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Personality")
		float IndependentVsCompliant = 0.3f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Personality")
		float IdealistVsPragmatist = 0.4f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Personality")
		float CasualVsProfessional = 0.2f;

	/// Gameplay: Skills
	// These should be class-related values and differ from character to character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Athletic")
		bool bFitness = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Armor")
		bool bLightArmor = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Charisma")
		bool bBluff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Charisma")
		bool bPersuasion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Charisma")
		bool bPerform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Charisma")
		bool bTaunt;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Combat")
		bool bMelee = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Combat")
		bool bSharpshooter = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Combat")
		bool bWeaponsPistols = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Combat")
		bool bWeaponsGrenades = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Tech Power")
		bool bTechUser; // TODO: testing https://www.youtube.com/watch?v=Nt4W1B8cKy8 should be set to false


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Investigation")
		bool bIntuition = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Investigation")
		bool bListen;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Medic")
		bool bFirstAid = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Skills\|Medic")
		bool bMedicine = false;

	
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
		int AmyLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Leveling")
		int AmySkillPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Leveling")
		float AmyCurrentXP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Leveling")
		float AmyNeededXP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Leveling")
		FName AmyClassName = "Engineer";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Leveling")
		FName AmyFirstName = "Amy";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Leveling")
		FName AmyCallsign = "'Phoenix'";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay\|Leveling")
		FName AmyLastName = "Carpenter";

	/// Gameplay: Inventory And Equipment
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay\|InventoryAndEquipment")
		TArray<F_ItemInfo> Inventory;

	/**
	 *	PUBLIC FUNCTIONS
	 */
	 /// Gameplay: Leveling
	UFUNCTION(BlueprintCallable, Category = "Gameplay\|Leveling")
		void AddXP(float AddedXP);

	UFUNCTION(BlueprintCallable, Category = "Gameplay\|Leveling")
		void AddSkillPoints();


	/// Gameplay: Inventory And Equipment
	// TODO: Confirm if this function does require const at the end as a "pure" function
	UFUNCTION(BlueprintCallable, Category = "Gameplay\|Inventory And Equipment")
		virtual bool HasSpaceInInventory() const;


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

	UFUNCTION(BlueprintCallable, Category = "Attributes\|Health")
		void UpdateHealth(float HealthChange);

	/** Check current Tech percentage in main status UI */
	UFUNCTION(BlueprintPure, Category = "Attributes\|Tech")
		float GetTech();

	/** Return Tech percentage for the UI to read */
	UFUNCTION(BlueprintPure, Category = "Attributes\|Tech")
		FText GetTechIntText();

	UFUNCTION()
		void SetTechValue();

	UFUNCTION()
		void SetTechState();

	UFUNCTION()
		void SetTechChange(float TechChange);

	UFUNCTION()
		void UpdateTech();


	/// Perception AI
	/*Hearing function - will be executed when we hear a Pawn*/
	UFUNCTION()
		void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);
protected:
	/**
	*	PROTECTED FUNCTIONS
	*/
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

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

	UFUNCTION(BlueprintCallable, Category = "Attributes\|Tech")
		void OnTechCast();
};
