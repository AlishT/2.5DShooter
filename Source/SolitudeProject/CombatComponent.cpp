// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Weapon.h"
#include "BaseCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

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
	}
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

	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("s_weapon"));
	if (HandSocket)
	{
		HandSocket->AttachActor(EquippedWeapon, Character->GetMesh());
	}

	EquippedWeapon->SetOwner(Character);
}

void UCombatComponent::Reload()
{
	if (CarriedAmmo > 0)
	{
		if (!Character) return;
		Character->PlayReloadMontage();
	}
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
		if (bCanFire && !EquippedWeapon->IsEmpty())
		{
			bCanFire = false;

			StartFireTimer();
		}
	}
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
	}

}

