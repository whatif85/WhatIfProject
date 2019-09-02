// Copyright FastForward Studios. All Rights Reserved.


//#include "GameFramework/LocalMessage.h"
//#include "GameFramework/EngineMessage.h"
//#include "Engine.h"

#include "BaseInteractable.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"

//#include "GameFramework/Actor.h"
#include "Characters/Squad/PlayerCharacter.h"



// Sets default values
ABaseInteractable::ABaseInteractable(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	/*if (HasAnyFlags(RF_ClassDefaultObject) && (ABaseInteractable::StaticClass() == GetClass()))
	{
		ABaseInteractable::__CustomDynamicClassInitialization(CastChecked<UDynamicClass>(GetClass()));
	}*/

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set components and their children
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	InteractableStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InteractableStaticMesh"));
	StaticInteractableArea = CreateDefaultSubobject<USphereComponent>(TEXT("StaticInteractableArea"));
	InteractableSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("InteractableSkeletalMesh"));
	SkeletalInteractableArea = CreateDefaultSubobject<USphereComponent>(TEXT("SkeletalInteractableArea"));

	RootComponent = DefaultSceneRoot;
	InteractableStaticMesh->AttachToComponent(DefaultSceneRoot, FAttachmentTransformRules::KeepRelativeTransform);
	StaticInteractableArea->AttachToComponent(InteractableStaticMesh, FAttachmentTransformRules::KeepRelativeTransform);
	InteractableSkeletalMesh->AttachToComponent(DefaultSceneRoot, FAttachmentTransformRules::KeepRelativeTransform);
	SkeletalInteractableArea->AttachToComponent(InteractableSkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform);
}

//// Called when the game starts or when spawned
//void ABaseInteractable::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//// Called every frame
//void ABaseInteractable::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void ABaseInteractable::OnComponentBeginOverlap_InteractableStaticMesh_Delegate(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	FHitResult const& SweepResult
)
{
	PointerToAnyUObject = OtherActor;

	// Dynamic Casts
	APlayerCharacter* pc = Cast<APlayerCharacter>(OtherActor);
	//TScriptInterface<IInteractInterface>* ii;
	IInteractInterface* ii = Cast<IInteractInterface>(OtherActor);

	// To determine if an actor implements an interface in both C++ and Blueprints
	if (OtherActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
	{
		// Whenever calling your interface functions in C++,
		// never call the direct functions, always use the one with the "Execute_" prefix
		// NOTE: You may need to compile before these functions will appear
		IInteractInterface::Execute_RegisterInteractable(pc->GetOwner(), this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Interactable Not Registered!"));
	}

	/*
	// Reverse engineering from Nativized code

	typedef FHitResult hr;
	hr& SweepResult = *const_cast<hr*>(&SweepResult);

	AActor* ComponentBoundEvent_OtherActor = OtherActor;

	//APlayerCharacter* OtherCharacter = Cast<APlayerCharacter>(ComponentBoundEvent_OtherActor);

	//pc = Cast<APlayerCharacter>(ComponentBoundEvent_OtherActor);
	bool bDynamicCastSuccess1 = (pc != nullptr);;

	if (bDynamicCastSuccess1 == false)
	{
		return;
	}

	if (pc && pc->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()) == true)
	{
		ii.SetObject(pc);

		void* lAddress = pc->GetInterfaceAddress(UInteractInterface::StaticClass());
		ii.SetInterface(lAddress);
	}
	else
	{
		ii.SetInterface(nullptr);
	}

	bool bDynamicCastSuccess2 = (ii != nullptr);;

	if (bDynamicCastSuccess2 == false)
	{
		return;
	}

	//if (::IsValid(ii))
	if (ii)
	{
		ii->RegisterInteractable(ii.GetObject(), this);

		//IInteractInterface::Execute_RegisterInteractable(ii.GetObject(), this);
	}

	return;
	*/
}

void ABaseInteractable::OnComponentEndOverlap_InteractableStaticMesh_Delegate(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	PointerToAnyUObject = OtherActor;

	// Dynamic Casts
	APlayerCharacter* pc = Cast<APlayerCharacter>(OtherActor);
	//TScriptInterface<IInteractInterface>* ii;
	IInteractInterface* ii = Cast<IInteractInterface>(OtherActor);

	// To determine if an actor implements an interface in both C++ and Blueprints
	if (OtherActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
	{
		// Whenever calling your interface functions in C++,
		// never call the direct functions, always use the one with the "Execute_" prefix
		// NOTE: You may need to compile before these functions will appear
		IInteractInterface::Execute_UnregisterInteractable(pc->GetOwner(), this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Interactable Not Unregistered!"));
	}
}

void ABaseInteractable::RegisterInteractable_Implementation(ABaseInteractable* Interactable)
{
	Interactable = this;
	UE_LOG(LogTemp, Warning, TEXT("Register Interactable"));
}

void ABaseInteractable::UnregisterInteractable_Implementation(ABaseInteractable* Interactable)
{
	Interactable = this;
	UE_LOG(LogTemp, Warning, TEXT("Unregister Interactable"));
}

void ABaseInteractable::Interact_Implementation(APlayerCharacter* PlayerInstigator)
{
}

void ABaseInteractable::PickUp_Implementation(F_ItemInfo ItemInfo)
{
}

void ABaseInteractable::SetHighlight(bool bIsHighlighted)
{
	if (::IsValid(InteractableStaticMesh))
	{
		InteractableStaticMesh->UPrimitiveComponent::SetRenderCustomDepth(bIsHighlighted);
	}
}

