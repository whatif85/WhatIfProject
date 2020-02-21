// Copyright FastForward Studios. All Rights Reserved.

// Tutorial: https://www.orfeasel.com/implement-ai-hearing-using-c/

#pragma once

#include "CoreMinimal.h"

#include "AIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "SquadAIController.generated.h"

/**
 * 
 */
UCLASS()
class WHATIFPROJECT_API ASquadAIController : public AAIController
{
	GENERATED_BODY()

protected:
	/*A Behavior tree component in order to be able to call specific functions like starting our BT*/
	UBehaviorTreeComponent* BehaviorTreeComp;

	/*A Blackboard component which will be used to initialize our Blackboard Values*/
	UBlackboardComponent* BlackboardComp;

	/*This property is used to find a certain key for our blackboard.*/
	UPROPERTY(EditDefaultsOnly)
		FName TargetKey = "SensedPawn";

public:
	/*Default Constructor*/
	ASquadAIController();

	/*Called when the AI Controller possesses a Pawn*/
	virtual void OnPossess(APawn* InPawn) override;

	/*Sets the new sensed target value inside our Blackboard values*/
	void SetSensedTarget(APawn* NewTarget);
};
