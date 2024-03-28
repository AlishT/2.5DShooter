// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "HealthComponent.h"
#include "CombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "SolitudeProject.h"
#include "BasePlayerController.h"
#include "CombatState.h"

ABaseCharacter::ABaseCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("WeaponComponent"));

	HealthComponent->OnHealthChanged.AddDynamic(this, &ABaseCharacter::HealthChanged);

	/*GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	
	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);*/

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	if (Movement)
	{
		Movement->bConstrainToPlane = true;
		Movement->SetPlaneConstraintNormal(FVector(0.f, 1.f, 0.f));
	}

	GrenadeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeMesh"));
	GrenadeMesh->SetupAttachment(GetMesh(), FName("s_grenade"));
	GrenadeMesh->SetVisibility(false);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	if (DefaultWeaponClass)
	{
		AWeapon* EnemyWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultWeaponClass, GetActorLocation(), GetActorRotation(), SpawnParams);

		CombatComponent->EquipWeapon(EnemyWeapon);
		CombatComponent->HidePrimaryWeapon();
	}
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
	PlayHitReactMontage();

	if (Health > 0) return;

	if (CombatComponent && CombatComponent->EquippedWeapon)
	{
		CombatComponent->EquippedWeapon->Dropped();
	}
	else if (CombatComponent && CombatComponent->GetHiddenWeapon())
	{
		CombatComponent->GetHiddenWeapon()->Dropped();
	}

	bEliminated = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->PlayAnimation(EliminatedAnim, false);
	//GetMesh()->SetSimulatePhysics(true);
	

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

void ABaseCharacter::PlayReloadMontage()
{
	if (!CombatComponent && !ReloadMontage) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && ReloadMontage)
	{
		AnimInstance->Montage_Play(ReloadMontage);
		FName SectionName;

		switch (CombatComponent->EquippedWeapon->GetWeaponType())
		{
		case EWeaponType::EWT_AssultRifle:
			SectionName = FName("Rifle");
			break;
		}
		
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ABaseCharacter::PlayHitReactMontage()
{
	if (!CombatComponent && !HitReactMontage) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		FName SectionName = FName("FromFromt");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ABaseCharacter::PlayGrenadeMontage()
{
	if (!CombatComponent && !GrenadeMontage) return;
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && GrenadeMontage)
	{
		AnimInstance->Montage_Play(GrenadeMontage);
		//FName SectionName = FName("Grenade");
		//AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ABaseCharacter::SetGrenadeMeshVisible(bool bVisible) const
{
	if (!GrenadeMesh) return;

	GrenadeMesh->SetVisibility(bVisible);
}

AWeapon* ABaseCharacter::GetEquippedWeapon() const
{
	if (CombatComponent == nullptr)
	{
		return nullptr;
	}

	return CombatComponent->EquippedWeapon;
}

ECombatState ABaseCharacter::GetCombatState() const
{
	if (!CombatComponent) return ECombatState::ECS_MAX;

	return CombatComponent->CombatState;
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
