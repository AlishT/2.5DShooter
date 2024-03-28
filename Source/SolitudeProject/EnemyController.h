// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;
class UAIPerceptionComponent;
/**
 * 
 */
UCLASS()
class SOLITUDEPROJECT_API AEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyController();

	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(BlueprintReadWrite, Category = "AI Behavior", meta = (AllowPrivateAccess = "true"))
	UBlackboardComponent* BlackboardComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "AI Behavior", meta = (AllowPrivateAccess = "true"))
	UBehaviorTreeComponent* BehaviorTreeComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "AIPerception")
	UAIPerceptionComponent* AIPerception = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	TSubclassOf<AActor> TargetClass;

protected:
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);

public:
	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const {
		return BlackboardComponent;
	}
};
