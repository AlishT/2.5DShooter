// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "MainCharacter.generated.h"

class AFollowCamera;
class AWeapon;
class AFollowCamera;

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
	TSubclassOf<AFollowCamera> FollowCameraClass = nullptr;

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
	class UInputAction* HideWeaponAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* FireAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* AimAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* SprintAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* ReloadAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* GrenadeAction = nullptr;
	
	UPROPERTY(Transient)
	AWeapon* PickapedWeapom = nullptr;

	UPROPERTY(Transient)
	AFollowCamera* FollowCamera = nullptr;

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
	void HideWeapon();
	UFUNCTION()
	void Fire();
	UFUNCTION()
	void Aiming();
	UFUNCTION()
	void Sprint();
	UFUNCTION()
	void ToJump();
	UFUNCTION()
	void Reload();
	UFUNCTION()
	void UseGrenade();
	
	UFUNCTION()
	void RotateCharacter();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE void SetOverlappingWeapon(AWeapon* Weapon) { PickapedWeapom = Weapon; }
	FORCEINLINE AFollowCamera* GetFollowCamera() { return FollowCamera; }
};
