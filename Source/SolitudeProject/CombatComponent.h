// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponTypes.h"
#include "CombatState.h"
#include "CombatComponent.generated.h"

class AWeapon;
class ABaseCharacter;
class ABasePlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOLITUDEPROJECT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

	friend ABaseCharacter;

public:	
	// Sets default values for this component's properties
	UCombatComponent();
	
private:
	UPROPERTY(Transient)
	ABaseCharacter* Character = nullptr;

	UPROPERTY(Transient)
	AWeapon* EquippedWeapon = nullptr;

	UPROPERTY(Transient)
	ABasePlayerController* PlayerController = nullptr;

	bool bAiming = false;

	UPROPERTY(EditDefaultsOnly, Category = "WalkSpeed")
	float BaseWalkSpeed = 600.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "WalkSpeed")
	float AimWalkSpeed = 300.f;

	bool bCanFire = true;

	float CurrentTime = 0.f;

	int32 CarriedAmmo = 0;

	TMap<EWeaponType, int32> CarriedAmmoMap;

	UPROPERTY()
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	//void SetCombatState();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FTimerHandle FireTimer;

	void StartFireTimer();

	void FireTimerFinished();

	void IntializeCarriedAmmo();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE AWeapon* GetEquippedWeapon() const { return EquippedWeapon; }

	void EquipWeapon(AWeapon* WeaponToEquip);

	void SetCarriedAmmoHUD();
	
	void Reload();

	UFUNCTION(BlueprintCallable)
	void FinishReloading();
		
	void SetAiming(const bool bIsAiming);

	bool GetAiming() const;

	void Fire();


};
