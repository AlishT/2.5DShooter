// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UHealthComponent;
class UCombatComponent;
class AProjectile;
class UAnimMontage;
class AWeapon;
class UAnimMontage;

UCLASS()
class SOLITUDEPROJECT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	UHealthComponent* HealthComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	UCombatComponent* CombatComponent = nullptr;

	FVector TargetLocation = FVector::ZeroVector;

	virtual void BeginPlay() override;

	void AimOffset(float DelataTime);

	float AO_Yaw = 0.f;
	
	float AO_Pitch = 0.f;
	
	FRotator StartingAimRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* FireWeaponMontage = nullptr;

public:	

	bool bSprinting = false;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void HealthChanged(float Health, float Armor);

	bool IsWeaponEquipeed();

	bool IsAiming();

	void PlayFireMontage(bool bAiming);

	FORCEINLINE float GetAOYaw() const { return AO_Yaw; }
	
	FORCEINLINE float GetAOPitch() const { return AO_Pitch; }

	AWeapon* GetEquippedWeapon() const;

};
