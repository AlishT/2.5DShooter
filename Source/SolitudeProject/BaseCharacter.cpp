// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "HealthComponent.h"
#include "CombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon.h"

ABaseCharacter::ABaseCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("WeaponComponent"));

	HealthComponent->OnHealthChanged.AddDynamic(this, &ABaseCharacter::HealthChanged);

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	
	if (Movement)
	{
		Movement->bConstrainToPlane = true;
		Movement->SetPlaneConstraintNormal(FVector(0.f, 1.f, 0.f));
	}
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//AimOffset(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::HealthChanged(float Health, float Armor)
{
	if (Health > 0)
	{
		return;
	}

	Destroy();
}

bool ABaseCharacter::IsWeaponEquipeed()
{
	return (CombatComponent && CombatComponent->EquippedWeapon);
}

bool ABaseCharacter::IsAiming()
{
	return (CombatComponent && CombatComponent->bAiming);
}

void ABaseCharacter::PlayFireMontage(bool bAiming)
{
	if (!CombatComponent && !FireWeaponMontage) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && FireWeaponMontage)
	{
		AnimInstance->Montage_Play(FireWeaponMontage);
		FName SectionName = (bAiming) ? FName("RifleAim") : FName("HipAim");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

AWeapon* ABaseCharacter::GetEquippedWeapon() const
{
	if (CombatComponent == nullptr)
	{
		return nullptr;
	}

	return CombatComponent->EquippedWeapon;
}

void ABaseCharacter::AimOffset(float DelataTime)
{
	if (!CombatComponent && !CombatComponent->EquippedWeapon) return;

	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	float Speed = Velocity.Size();

	bool bIsInAir = GetCharacterMovement()->IsFalling();

	if (Speed == 0.f && !bIsInAir) // standing still, not jumping
	{
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		
		AO_Yaw = DeltaAimRotation.Yaw;
		//bUseControllerRotationYaw = false;
	}

	if (Speed > 0.f || bIsInAir) //running or jumping
	{
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);

		AO_Yaw = 0.f;
		//bUseControllerRotationYaw = true;
	}

	AO_Pitch = GetBaseAimRotation().Pitch;
}
