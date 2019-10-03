// Copyright FastForward Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Characters/Squad/PlayerCharacter.h"

#include "MedKit.generated.h"

UCLASS()
class WHATIFPROJECT_API AMedKit : public AActor
{
	GENERATED_BODY()
	
public:	
	AMedKit();

	UPROPERTY(EditAnywhere)
		APlayerCharacter* rPlayerCharacter;

	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
};
