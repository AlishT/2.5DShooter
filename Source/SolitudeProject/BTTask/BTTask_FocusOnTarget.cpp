// Fill out your copyright notice in the Description page of Project Settings.


#include "SolitudeProject/BTTask/BTTask_FocusOnTarget.h"
#include "SolitudeProject/EnemyController.h"
#include "SolitudeProject/MainCharacter.h"
#include "SolitudeProject/CombatComponent.h"
#include "SolitudeProject/Weapon.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_FocusOnTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!OwnerComp.GetAIOwner()) return EBTNodeResult::Failed;

	AEnemyController* AIConrtoler = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	if (AIConrtoler)
	{
		AMainCharacter* Player = Cast<AMainCharacter>(AIConrtoler->GetBlackboardComponent()->GetValueAsObject(TEXT("AttackeTarget")));
		if (Player)
		{
			AIConrtoler->SetFocus(Player);
		}
	}
	return EBTNodeResult::Succeeded;
}
