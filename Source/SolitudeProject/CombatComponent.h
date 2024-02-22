// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponTypes.h"
#include "CombatComponent.generated.h"

class AWeapon;
class ABaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOLITUDEPROJECT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

	friend ABaseCharacter;

public:	
	// Sets default values for this component's properties
	UCombatComponent();
	
	UPROPERTY()
	AWeapon* EquippedWeapon = nullptr;
private:
	ABaseCharacter* Character = nullptr;

	bool bAiming = false;

	UPROPERTY(EditDefaultsOnly, Category = "WalkSpeed")
	float BaseWalkSpeed = 600.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "WalkSpeed")
	float AimWalkSpeed = 300.f;

	bool bCanFire = true;

	float CurrentTime = 0.f;

	int32 CarriedAmmo = 30;

	TMap<EWeaponType, int32> VarriedAmmoMap;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FTimerHandle FireTimer;

	void StartFireTimer();

	void FireTimerFinished();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void EquipWeapon(AWeapon* WeaponToEquip);
	void Reload();
		
	void SetAiming(const bool bIsAiming);

	bool GetAiming() const;

	void Fire();
};
