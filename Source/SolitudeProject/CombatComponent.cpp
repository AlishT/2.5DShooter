// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Weapon.h"
#include "BaseCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BasePlayerController.h"
#include "TimerManager.h"
#include "CombatState.h"
#include "Sound/SoundCue.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

		PlayerController = Character->GetController<ABasePlayerController>();
	}

	IntializeCarriedAmmo();
}

// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	Character = Cast<ABaseCharacter>(GetOwner());

	if (!WeaponToEquip || !Character) return;
	
	if (EquippedWeapon && !SecondaryWeapon)
	{
		EquipSecondaryWeapon(WeaponToEquip);
	}
	else
	{
		EquipPrimaryWeapon(WeaponToEquip);
	}
}

void UCombatComponent::SwapWeapon()
{
	AWeapon* TempWeapon = EquippedWeapon;
	EquippedWeapon = SecondaryWeapon;
	SecondaryWeapon = TempWeapon;

	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	AttachedWeapon(EquippedWeapon, FName("s_weapon"));
	EquippedWeapon->SetAmmoHUD();
	SetCarriedAmmoHUD();
	
	SecondaryWeapon->SetWeaponState(EWeaponState::EWS_EquippedSecondary);
	FName SocketName = SetSockedName(SecondaryWeapon->GetWeaponType());
	AttachedWeapon(SecondaryWeapon, SocketName);
}

void UCombatComponent::EquipPrimaryWeapon(AWeapon* WeaponToEquip)
{
	if (!WeaponToEquip) return;

	if (EquippedWeapon)
	{
		EquippedWeapon->Dropped();
	}

	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	EquippedWeapon->SetAmmoHUD();

	AttachedWeapon(WeaponToEquip, FName("s_weapon"));

	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
	}

	if (EquippedWeapon->IsEmpty())
	{
		Reload();
	}
	 
	SetCarriedAmmoHUD();

}

void UCombatComponent::EquipSecondaryWeapon(AWeapon* WeaponToEquip)
{
	if (!WeaponToEquip) return;

	SecondaryWeapon = WeaponToEquip;
	SecondaryWeapon->SetWeaponState(EWeaponState::EWS_EquippedSecondary);
	FName SocketName = SetSockedName(SecondaryWeapon->GetWeaponType());
	AttachedWeapon(WeaponToEquip, SocketName);
}

FName UCombatComponent::SetSockedName(EWeaponType Type)
{
	FName SocketName;

	switch (Type)
	{
	case EWeaponType::EWT_Pistol:
	case EWeaponType::EWT_MashineGun:
		SocketName = "PistolSocket";
		break;
	case EWeaponType::EWT_AssultRifle:
		SocketName = "MiddleWeaponSocked";
		break;
	case EWeaponType::EWT_RocketLauncher:
		SocketName = "HeaveWeaponSocket";
		break;
	}

	return SocketName;
}

void UCombatComponent::AttachedWeapon(AWeapon* WeaponToEquip, FName SockedName)
{
	if (WeaponToEquip->EquippedSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, WeaponToEquip->EquippedSound, Character->GetActorLocation());
	}

	const USkeletalMeshSocket* WeaponSocket = Character->GetMesh()->GetSocketByName(SockedName);
	if (WeaponSocket)
	{
		WeaponSocket->AttachActor(WeaponToEquip, Character->GetMesh());
	}

	WeaponToEquip->SetOwner(Character);
}


void UCombatComponent::SetAiming(const bool bIsAiming)
{
	bAiming = bIsAiming;

	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = (bIsAiming) ? AimWalkSpeed : BaseWalkSpeed;
	}
}

bool UCombatComponent::GetAiming() const
{
	return bAiming;
}

void UCombatComponent::Fire()
{
	if (EquippedWeapon->bAutomatic)
	{
		if (bCanFire && !EquippedWeapon->IsEmpty() && !EquippedWeapon->IsTargetOwner())
		{
			bCanFire = false;

			StartFireTimer();
		}
	}
}

bool UCombatComponent::ShoodSwapWeapons()
{
	return EquippedWeapon && SecondaryWeapon;
}

void UCombatComponent::StartFireTimer()
{
	if (!EquippedWeapon && !Character) return;

	Character->GetWorldTimerManager().SetTimer(FireTimer, this, &UCombatComponent::FireTimerFinished, EquippedWeapon->FireDelay);
}

void UCombatComponent::FireTimerFinished()
{
	bCanFire = true;

	if (EquippedWeapon->bAutomatic)
	{
		EquippedWeapon->Shoot(Character->GetHitTarget());
		Character->PlayFireMontage(GetAiming());
	}

	if (EquippedWeapon->IsEmpty())
	{
		Reload();
	}

}

void UCombatComponent::IntializeCarriedAmmo()
{
	CarriedAmmoMap.Emplace(EWeaponType::EWT_Pistol, MaxPistolAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_AssultRifle, MaxARAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_MashineGun, MaxMGAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_RocketLauncher, MaxRocketAmmo);
}

void UCombatComponent::Reload()
{
	if (CarriedAmmo > 0 && EquippedWeapon && !EquippedWeapon->IsFullCapasity() && CombatState != ECombatState::ECS_Reloading)
	{
		if (!Character) return;

		CombatState = ECombatState::ECS_Reloading;
		Character->PlayReloadMontage();
		bCanFire = false;
	}
}

int32 UCombatComponent::AmountToReloaud()
{
	if (!EquippedWeapon) return 0;

	int32 RoomInMag = EquippedWeapon->GetRoomInMag();
	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		int32 AmountCarried = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
		int32 Least = FMath::Min(RoomInMag, AmountCarried);
		return FMath::Clamp(RoomInMag, 0, Least);

	}
	return 0;
}

void UCombatComponent::FinishReloading()
{
	CombatState = ECombatState::ECS_Unoccupied;

	UpdateAmmoValue();
	bCanFire = true;
}

void UCombatComponent::UpdateAmmoValue()
{
	if (!EquippedWeapon) return;

	int32 ReloadAmount = AmountToReloaud();
	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		CarriedAmmoMap[EquippedWeapon->GetWeaponType()] -= ReloadAmount;
	}
	EquippedWeapon->AddAmmo(-ReloadAmount);
	SetCarriedAmmoHUD();
}

void UCombatComponent::SetCarriedAmmoHUD()
{
	if (Character)
	{
		PlayerController = (!PlayerController) ? Character->GetController<ABasePlayerController>() : PlayerController;

		if (PlayerController && CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
		{
			PlayerController->SetHUDCarriedAmmo(CarriedAmmoMap[EquippedWeapon->GetWeaponType()]);
		}
	}
}
