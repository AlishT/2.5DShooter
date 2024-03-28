// Fill out your copyright notice in the Description page of Project Settings.


#include "SolitudeProject/BTTask/BTTask_DistanceAttacke.h"
#include "SolitudeProject/EnemyController.h"
#include "SolitudeProject/EnemyCharacter.h"
#include "SolitudeProject/CombatComponent.h"
#include "SolitudeProject/Weapon.h"

EBTNodeResult::Type UBTTask_DistanceAttacke::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!OwnerComp.GetAIOwner()) return EBTNodeResult::Failed;

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	
	if (Enemy)
	{
		Enemy->GetCombatComponent()->Fire();
	}

	return EBTNodeResult::Succeeded;
}
