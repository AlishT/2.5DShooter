// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EnemyCharacter.generated.h"

class AWeapon;
/**
 * 
 */
UCLASS()
class SOLITUDEPROJECT_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Default")
	TSubclassOf<AWeapon> EnemyWeaponClass;

	virtual void BeginPlay()override;
};
