// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EnemyCharacter.generated.h"

class AWeapon;
class UBehaviorTree;
class AEnemyController;
class UPawnSensingComponent;

/**
 * 
 */
UCLASS()
class SOLITUDEPROJECT_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

protected:
	virtual void BeginPlay()override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree = nullptr;

	//UPROPERTY(EditDefaultsOnly, Category = "Default")
	//UPawnSensingComponent* PawnSensing = nullptr;

	//UPROPERTY(EditDefaultsOnly, Category = "Default")
	//TSubclassOf<AActor> TargetClass;

	UPROPERTY(EditAnywhere, Category = "BehaviorTree", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	FVector PatrolPoint = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "BehaviorTree", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	FVector PatrolPoint2 = FVector::ZeroVector;

	UPROPERTY(Transient)
	AEnemyController* EnemyController = nullptr;

	//UFUNCTION()
	//void OnSeeEnemy(APawn* Pawn);

public:
	FORCEINLINE UBehaviorTree* GetBehavierTree() const { return BehaviorTree; }
};
