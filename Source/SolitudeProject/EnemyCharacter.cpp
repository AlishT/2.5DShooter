// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "CombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon.h"

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	if (EnemyWeaponClass)
	{
		AWeapon* EnemyWeapon = GetWorld()->SpawnActor<AWeapon>(EnemyWeaponClass, GetActorLocation(), GetActorRotation(), SpawnParams);

		CombatComponent->EquipWeapon(EnemyWeapon);
	}
}
