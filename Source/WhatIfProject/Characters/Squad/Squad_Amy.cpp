// Copyright FastForward Studios. All Rights Reserved.


#include "Squad_Amy.h"

// Sets default values
ASquad_Amy::ASquad_Amy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASquad_Amy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASquad_Amy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASquad_Amy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

