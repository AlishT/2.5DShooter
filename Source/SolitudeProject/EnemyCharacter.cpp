// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "CombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "MainCharacter.h"
#include "EnemyController.h"

AEnemyCharacter::AEnemyCharacter()
{
	GetCharacterMovement()->bConstrainToPlane = false;

	//PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	//PawnSensing->OnSeePawn.AddDynamic(this, &AEnemyCharacter::OnSeeEnemy);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	const FVector WorldPatrolPoint = UKismetMathLibrary::TransformLocation(GetActorTransform(), PatrolPoint);

	const FVector WorldPatrolPoint2 = UKismetMathLibrary::TransformLocation(GetActorTransform(), PatrolPoint2);

	EnemyController = GetController<AEnemyController>();
	
	if (EnemyController)
	{
		EnemyController->GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolPoint"), WorldPatrolPoint);

		EnemyController->GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolPoint2"), WorldPatrolPoint2);

		EnemyController->RunBehaviorTree(BehaviorTree);
	}

	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AEnemyController* AIController = GetController<AEnemyController>(); 
	
	if (AIController && AIController->GetFocusActor() )
	{
		TargetLocation = AIController->GetFocusActor()->GetActorLocation();

		FVector Direction = TargetLocation - GetActorLocation();

		FRotator Rotation = Direction.Rotation();
		Rotation.Normalize();

		AO_Pitch = Rotation.Pitch;
	}

	if (bEliminated)
	{
		CurrentTime += DeltaTime;

		if (CurrentTime >= BodyLifeTime)
		{
			Destroy();
		}
	}
}

/*void AEnemyCharacter::OnSeeEnemy(APawn* Pawn)
{
	if (!Pawn) return;

	if (Pawn->IsA(TargetClass))
	{
		if (EnemyController && TargetClass)
		{
			EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsSeeEnemy"), true);
			EnemyController->GetBlackboardComponent()->SetValueAsObject(TEXT("AttackeTarget"), Pawn);
		}

		//CombatComponent->SetAiming(true);

		TargetLocation = Pawn->GetActorLocation();
	}
}*/
