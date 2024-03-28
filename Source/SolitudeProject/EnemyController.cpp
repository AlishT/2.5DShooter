// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyCharacter.h"
#include "Perception/AIPerceptionComponent.h"

AEnemyController::AEnemyController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	check(BlackboardComponent);

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	check(BehaviorTreeComponent);

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnTargetDetected);
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!InPawn) return;

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(InPawn);

	if (Enemy)
	{
		if (Enemy->GetBehavierTree())
		{
			BlackboardComponent->InitializeBlackboard(*(Enemy->GetBehavierTree()->BlackboardAsset));

			BehaviorTreeComponent->StartTree(*(Enemy->GetBehavierTree()));
		}
	}
}

void AEnemyController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	if (Actor->IsA(TargetClass))
	{
		//ABaseCharacter* AttackeTarget = Cast<ABaseCharacter>(Actor);
		if (Stimulus.WasSuccessfullySensed())
		{
			SetFocus(Actor);

			BlackboardComponent->SetValueAsObject(TEXT("AttackeTarget"), Actor);
			BlackboardComponent->SetValueAsBool(TEXT("IsSeeEnemy"), true);
			BlackboardComponent->SetValueAsVector(TEXT("TargetLocation"), Actor->GetActorLocation());
		}
		else
		{
			BlackboardComponent->ClearValue(TEXT("AttackeTarget"));
		}
	}
	
}
