// Copyright FastForward Studios. All Rights Reserved.


#include "Squad_Amy.h"
#include "Characters/AI/SquadAIController.h"

#include "Classes/Components/SkeletalMeshComponent.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"

// Sets default values
ASquad_Amy::ASquad_Amy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Initializing Pawn Sensing Component
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	BehaviorTree = CreateDefaultSubobject<UBehaviorTree>(TEXT("Ref_BehaviorTree"));

	// Add Skeletal Mesh Sockets
	BaseTorso = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Base Torso"));
	Shoulders = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shoulders"));
	Waist = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Waist"));
	Legs1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs1"));

	// Amy's starting conditions
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	HealthPercentage = 1.0f; // 100%
	bCanBeDamaged = true;

	CurrentStamina = 100.0f;
	CurrentTech = 100.0f;

	MaxTech = 100.0f;
	CurrentTech = MaxTech;
	TechPercentage = 1.0f;
	PreviousTech = TechPercentage;
	TechValue = 0.0f; // not casting any phlebotinum charges
	bTechUser = true;

	// TODO: this will be moved to a separate function that will trigger only when this character can use dark energy skills for the 1st time
	if (TechCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineCallbackEnd;

		TimelineCallback.BindUFunction(this, FName("SetTechValue"));
		TimelineCallbackEnd.BindUFunction(this, FName("SetTechState"));

		AttributeTimeline = NewObject<UTimelineComponent>(this, FName("Tech Animation"));
		AttributeTimeline->AddInterpFloat(TechCurve, TimelineCallback);
		AttributeTimeline->SetTimelineFinishedFunc(TimelineCallbackEnd);
		AttributeTimeline->RegisterComponent();

		//AttributeTimeline.AddInterpFloat(TechCurve, TimelineCallback);
		//AttributeTimeline.SetTimelineFinishedFunc(TimelineCallbackEnd);
	}

	AmyLevel = 1;
	AmySkillPoints = 3;
	AmyCurrentXP = 0.0f;
	AmyNeededXP = 100.0f;
}

// Called when the game starts or when spawned
void ASquad_Amy::BeginPlay()
{
	Super::BeginPlay();
	
	if (PawnSensingComp != nullptr)
	{
		//Registering the delegate which will fire when we hear something
		PawnSensingComp->OnHearNoise.AddDynamic(this, &ASquad_Amy::OnHearNoise);
	}

	// Done at Blueprint
	//PawnSensingComp->bSeePawns = false;
}

// Called every frame
void ASquad_Amy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//AttributeTimeline->TickTimeline(DeltaTime);
	if (AttributeTimeline != nullptr)
	{
		AttributeTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	}
}


/**
*	LEVELING
*/
void ASquad_Amy::AddXP(float AddedXP)
{
	AmyCurrentXP += AddedXP;

	if (AmyCurrentXP >= AmyNeededXP)
	{
		AmyCurrentXP -= AmyNeededXP;
		AmyLevel++;

		AddSkillPoints();

		AmyNeededXP *= 2.0f;
	}
}

void ASquad_Amy::AddSkillPoints()
{
	AmySkillPoints += 3;
}


/**
*	INVENTORY AND EQUIPMENT
*/
bool ASquad_Amy::HasSpaceInInventory() const
{
	bool bReturnValue{};
	int32 CallFunc_Array_Length_bReturnValue{};
	bool CallFunc_Less_IntInt_bReturnValue{};

	return bReturnValue;
}


/**
*	ATTRIBUTES
*/
float ASquad_Amy::TakeDamage(
	float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser
)
{
	bCanBeDamaged = false;

	UpdateHealth(-DamageAmount);
	SetDamageTimer();

	return DamageAmount;
}

void ASquad_Amy::OnTechCast()
{
	// TODO: Characters are 3rd person only
}

float ASquad_Amy::GetHealth()
{
	return HealthPercentage;
}

FText ASquad_Amy::GetHealthIntText()
{
	int32 HitPoints = FMath::RoundHalfFromZero(HealthPercentage * 100);
	FString HP_String = FString::FromInt(HitPoints);
	FString Health_HUD = HP_String + FString(TEXT("%"));
	FText HP_Text = FText::FromString(Health_HUD);

	return HP_Text;
}

void ASquad_Amy::SetDamageTimer()
{
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &ASquad_Amy::SetDamageState, 2.0f, false);
}

void ASquad_Amy::SetDamageState()
{
	bCanBeDamaged = true;
}

void ASquad_Amy::UpdateHealth(float HealthChange)
{
	CurrentHealth += HealthChange;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);

	HealthPercentage = CurrentHealth / MaxHealth;
}


float ASquad_Amy::GetTech()
{
	return TechPercentage;
}

FText ASquad_Amy::GetTechIntText()
{
	int32 TechPoints = FMath::RoundHalfFromZero(TechPercentage * 100);
	FString Tech_String = FString::FromInt(TechPoints);
	FString Max_Tech_String = FString::FromInt(MaxTech);
	FString Tech_HUD = Tech_String + FString(TEXT("/") + Max_Tech_String);
	FText Tech_Text = FText::FromString(Tech_HUD);

	return Tech_Text;
}

void ASquad_Amy::SetTechValue()
{
	TimelineValue = AttributeTimeline->GetPlaybackPosition();

	CurveFloatValue = PreviousTech + TechValue * TechCurve->GetFloatValue(TimelineValue);

	// Prevent value to go less than 0 or above 1
	CurrentTech = FMath::Clamp(CurveFloatValue * MaxHealth, 0.0f, MaxTech);
	TechPercentage = FMath::Clamp(CurveFloatValue, 0.0f, 1.0f);
}

void ASquad_Amy::SetTechState()
{
	// TODO: This variable should be changed to a temporary state rather than merely be a DE user
	bTechUser = true;

	TechValue = 0.0f;

	if (TechDefaultMaterial)
	{
		HandHeldItem->SetMaterial(0, TechDefaultMaterial);
	}
}

void ASquad_Amy::SetTechChange(float TechCost)
{
	// TODO: This variable should be changed to a temporary state rather than merely be a DE user
	bTechUser = false;
	PreviousTech = TechPercentage;
	TechValue = (TechCost / MaxTech);

	if (TechBurnoutMaterial)
	{
		HandHeldItem->SetMaterial(0, TechBurnoutMaterial);
	}

	if (AttributeTimeline != nullptr)
	{
		AttributeTimeline->PlayFromStart();
	}
}

void ASquad_Amy::UpdateTech()
{
	PreviousTech = TechPercentage;
	TechPercentage = CurrentTech / MaxTech;
	TechValue = 1.0f;

	if (AttributeTimeline != nullptr)
	{
		AttributeTimeline->PlayFromStart();
	}
}

void ASquad_Amy::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
	ASquadAIController* _AIController = Cast<ASquadAIController>(GetController());

	//We don't want to hear ourselves
	if (_AIController && PawnInstigator != this)
	{
		//Updates our target based on what we've heard.
		_AIController->SetSensedTarget(PawnInstigator);
	}
}