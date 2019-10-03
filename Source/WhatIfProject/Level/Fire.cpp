// Copyright FastForward Studios. All Rights Reserved.


#include "Fire.h"

#include "Particles/ParticleSystemComponent.h"

#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"


AFire::AFire()
{
	rFireBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Fire Box Component"));
	rFireBoxComponent->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	RootComponent = rFireBoxComponent;

	FireParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Fire"));
	FireParticles->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	FireParticles->SetupAttachment(RootComponent);

	rFireBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFire::OnOverlapBegin);
	rFireBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AFire::OnEndOverlap);

	bCanApplyDamage = false;
}

void AFire::OnOverlapBegin(
	class UPrimitiveComponent* OverlappedComponent,
	class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
	)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		bCanApplyDamage = true;

		rSquadCharacter = Cast<AActor>(OtherActor);
		Hit = SweepResult;

		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AFire::ApplyHeatDamage, 2.2f, true, 0.0f);
	}
}

void AFire::OnEndOverlap(
	class UPrimitiveComponent* OverlappedComponent,
	class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
	)
{
	bCanApplyDamage = false;

	GetWorldTimerManager().ClearTimer(FireTimerHandle);
}

void AFire::ApplyHeatDamage()
{
	if (bCanApplyDamage == true)
	{
		UGameplayStatics::ApplyPointDamage(rSquadCharacter, 20.0f, GetActorLocation(), Hit, nullptr, this, HeatDamage);
	}
}