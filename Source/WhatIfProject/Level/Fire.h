// Copyright FastForward Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/BoxComponent.h"
#include "Particles/ParticleSystem.h"

#include "Fire.generated.h"

UCLASS()
class WHATIFPROJECT_API AFire : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFire();

	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* FireParticles;

	UPROPERTY(EditAnywhere)
		UBoxComponent* rFireBoxComponent;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UDamageType> HeatDamage;

	UPROPERTY(EditAnywhere)
		AActor* rSquadCharacter;

	UPROPERTY(EditAnywhere)
		FHitResult Hit;

	bool bCanApplyDamage;

	FTimerHandle FireTimerHandle;

	UFUNCTION()
		void OnOverlapBegin(
			class UPrimitiveComponent* OverlappedComponent,
			class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult &SweepResult
		);

	UFUNCTION()
		void OnEndOverlap(
			class UPrimitiveComponent* OverlappedComponent,
			class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex
		);

	UFUNCTION()
		void ApplyHeatDamage();
};
