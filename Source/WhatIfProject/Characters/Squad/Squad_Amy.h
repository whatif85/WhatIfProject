// Copyright FastForward Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Squad_Amy.generated.h"

UCLASS()
class WHATIFPROJECT_API ASquad_Amy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASquad_Amy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
