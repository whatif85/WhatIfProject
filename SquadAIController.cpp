// Copyright FastForward Studios. All Rights Reserved.


#include "SquadAIController.h"
#include "Characters/Squad/Squad_Amy.h"


ASquadAIController::ASquadAIController()
{
	//Initialize our components
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));

}

void ASquadAIController::OnPossess(APawn* InPawn)
{
	//Super::Possess(InPawn);

	ASquad_Amy* _Amy = Cast<ASquad_Amy>(InPawn);
	
	// If our character is valid and has a valid Behavior Tree,
	// Initialize the values of the Blackboard and start the tree
	if (_Amy && _Amy->BehaviorTree->BlackboardAsset)
	{
		//Initialize the blackboard values
		BlackboardComp->InitializeBlackboard(*_Amy->BehaviorTree->BlackboardAsset);

		//Start the tree
		BehaviorTreeComp->StartTree(*_Amy->BehaviorTree);

	}

}

void ASquadAIController::SetSensedTarget(APawn* NewTarget)
{
	//Set a new target to follow
	if (BlackboardComp) BlackboardComp->SetValueAsObject(TargetKey, NewTarget);
}