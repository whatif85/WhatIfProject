// Copyright FastForward Studios. All Rights Reserved.


#include "MedKit.h"

#include "Characters/Squad/PlayerCharacter.h"


AMedKit::AMedKit()
{
	OnActorBeginOverlap.AddDynamic(this, &AMedKit::OnOverlapBegin);
}

void AMedKit::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (OtherActor != nullptr && OtherActor != this)
	{
		rPlayerCharacter = Cast<APlayerCharacter>(OtherActor);

		if (rPlayerCharacter != nullptr && rPlayerCharacter->GetHealth() < 1.0f)
		{
			rPlayerCharacter->UpdateHealth(25.0f);

			Destroy();
		}
	}
}

