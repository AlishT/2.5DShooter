// Fill out your copyright notice in the Description page of Project Settings.


#include "SolitudeProject/BTTask/MyBTTask_TakeWeapon.h"
#include "SolitudeProject/EnemyController.h"
#include "SolitudeProject/EnemyCharacter.h"
#include "SolitudeProject/CombatComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UMyBTTask_TakeWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!OwnerComp.GetAIOwner()) return EBTNodeResult::Failed;

	UE_LOG(LogTemp, Warning, TEXT("See You"));

	AEnemyController* AIController = Cast<AEnemyController>(OwnerComp.GetAIOwner());

	if (AIController)
	{
		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AIController->GetPawn());
		
		if (Enemy && Enemy->GetCombatComponent())
		{
			Enemy->GetCombatComponent()->TakeHiddenWeapon();
			AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsTakeWeapon"), true);
		}
	}

	return EBTNodeResult::Succeeded;
}
