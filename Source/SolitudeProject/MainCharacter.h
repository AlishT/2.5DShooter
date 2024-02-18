// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "MainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AWeapon;

/**
 * 
 */
UCLASS()
class SOLITUDEPROJECT_API AMainCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	USpringArmComponent* CameraBoom = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	UCameraComponent* PlayerCamera = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputMappingContext* MovingMC = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* MoveAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* LookAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* JumpAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* CrouchAction = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* WeaponEquipAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* FireAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* AimAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* SprintAction = nullptr;

	AWeapon* PickapedWeapom = nullptr;

	float DirectionValue = 0.f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void Crouching();
	UFUNCTION()
	void EquipWeapon();
	UFUNCTION()
	void Fire();
	UFUNCTION()
	void Aiming();
	UFUNCTION()
	void Sprint();
	UFUNCTION()
	void ToJump();
	
	UFUNCTION()
	void RotateCharacter();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE void SetOverlappingWeapon(AWeapon* Weapon) { PickapedWeapom = Weapon; }

};
