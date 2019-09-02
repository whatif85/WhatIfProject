// Copyright FastForward Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/BlueprintSupport.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "Interactables/InteractInterface.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Interactables/Items/F_ItemInfo.h"

//#include "GameFramework/Actor.h"
//#include "Characters/Squad/PlayerCharacter.h"
//#include "Components/SphereComponent.h"

/// Forward Declarations
class USphereComponent;
class USkeletalMeshComponent;
class UStaticMeshComponent;
class USceneComponent;
//class ABaseInteractable;
class UPrimitiveComponent;
//class ABaseItem;
class AActor;
class APlayerCharacter;

#include "BaseInteractable.generated.h"


UCLASS(config = Engine, Blueprintable, BlueprintType)
class ABaseInteractable : public AActor, public IInteractInterface
{
public:
	GENERATED_BODY()

		// Sets default values for this actor's properties
		ABaseInteractable(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	//ABaseInteractable();

	//// Called every frame
	//virtual void Tick(float DeltaTime) override;

	//// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	UObject* PointerToAnyUObject;

	UPROPERTY(BlueprintReadWrite, NonTransactional)
		USceneComponent* DefaultSceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NonTransactional)
		UStaticMeshComponent* InteractableStaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NonTransactional)
		USphereComponent* StaticInteractableArea;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NonTransactional)
		USkeletalMeshComponent* InteractableSkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NonTransactional)
		USphereComponent* SkeletalInteractableArea;

	// TODO: Unknown function
	//ABaseInteractable(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Overlap events
	UFUNCTION(BlueprintCallable, Category = "Interactable")
		virtual void OnComponentBeginOverlap_InteractableStaticMesh_Delegate(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			FHitResult const& SweepResult
		);

	UFUNCTION()
		virtual void OnComponentEndOverlap_InteractableStaticMesh_Delegate(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex
		);

	// Interface Functions
	// TODO: Source: https://wiki.unrealengine.com/Interfaces_in_C%2B%2B
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interfaces\|Interactable")
		void RegisterInteractable(ABaseInteractable* Interactable); // prototype
		virtual void RegisterInteractable_Implementation(ABaseInteractable* Interactable) /*override*/; // declaration

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interfaces\|Interactable")
		void UnregisterInteractable(ABaseInteractable* Interactable);
		virtual void UnregisterInteractable_Implementation(ABaseInteractable* Interactable);


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interfaces\|Interactable")
		void Interact(APlayerCharacter* PlayerInstigator);
		virtual void Interact_Implementation(APlayerCharacter* PlayerInstigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interfaces\|Interactable")
		void PickUp(F_ItemInfo ItemInfo);
	virtual void PickUp_Implementation(F_ItemInfo ItemInfo);


	/*
	UFUNCTION(BlueprintCallable)
		virtual void I_Converse(F_CreatureInfo CreatureInfo);
	*/

	UFUNCTION(BlueprintCallable)
		virtual void SetHighlight(bool bIsHighlighted);
};
