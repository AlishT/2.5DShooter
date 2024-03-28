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
class AProjectile;

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
	AWeapon* SecondaryWeapon = nullptr;

	UPROPERTY(Transient)
	AWeapon* HiddenWeapon = nullptr;

	UPROPERTY(Transient)
	ABasePlayerController* PlayerController = nullptr;

	UPROPERTY()
	bool bAiming = false;

	UPROPERTY(EditDefaultsOnly, Category = "WalkSpeed")
	float BaseWalkSpeed = 600.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "WalkSpeed")
	float AimWalkSpeed = 300.f;

	UPROPERTY()
	bool bCanFire = true;

	UPROPERTY()
	float CurrentTime = 0.f;

	UPROPERTY()
	int32 CarriedAmmo = 60;

	UPROPERTY(Transient)
	TMap<EWeaponType, int32> CarriedAmmoMap;

	UPROPERTY(EditDefaultsOnly, Category = "CarriedAmmo")
	int32 MaxARAmmo = 60;

	UPROPERTY(EditDefaultsOnly, Category = "CarriedAmmo")
	int32 MaxPistolAmmo = 30;

	UPROPERTY(EditDefaultsOnly, Category = "CarriedAmmo")
	int32 MaxMGAmmo = 60;

	UPROPERTY(EditDefaultsOnly, Category = "CarriedAmmo")
	int32 MaxRocketAmmo = 2;

	UPROPERTY()
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	UPROPERTY(EditDefaultsOnly, Category = "GrenateClass")
	TSubclassOf<AProjectile> GrenadeProjectileClass;

	//void SetCombatState();

	void AttachedWeapon(AWeapon* WeaponToEquip, FName SockedName);

	void EquipPrimaryWeapon(AWeapon* WeaponToEquip);

	void EquipSecondaryWeapon(AWeapon* WeaponToEquip);

	FName SetSockedName(EWeaponType Type);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FTimerHandle FireTimer;

	void StartFireTimer();

	void FireTimerFinished();

	void IntializeCarriedAmmo();

	void UpdateAmmoValue();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE AWeapon* GetEquippedWeapon() const { return EquippedWeapon; }
	FORCEINLINE AWeapon* GetHiddenWeapon() const { return HiddenWeapon; }

	void EquipWeapon(AWeapon* WeaponToEquip);

	void SwapWeapon();

	void HidePrimaryWeapon();

	void TakeHiddenWeapon();

	void SetAmmoHUD();

	void SetCarriedAmmoHUD();
	
	void Reload();

	int32 AmountToReloaud();

	bool IsCarriedAmmo(EWeaponType Type);

	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	UFUNCTION(BlueprintCallable)
	void FinishGrenadeThrow();
		
	void SetAiming(const bool bIsAiming);

	bool GetAiming() const;
	
	UFUNCTION(BlueprintCallable)
	void Fire();

	void UseGrenade();

	bool ShoodSwapWeapons();
};
