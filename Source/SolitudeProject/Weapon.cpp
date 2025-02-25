// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "MainCharacter.h"
#include "Projectile.h"
#include "Engine/SkeletalMeshSocket.h"
#include "CombatComponent.h"
#include "Animation/AnimationAsset.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "BasePlayerController.h"
#include "TimerManager.h"
#include "BasePlayerController.h"
#include "FollowCamera.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WidjetCollision"));
	PickupCollision->SetupAttachment(RootComponent);
	
	PickupCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PickupCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);

	PickupCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereOverlap);
	PickupCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnEndSphereOverlap);

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	ShowPickupWidget(false);

	Ammo = MagCapasity;

}

void AWeapon::OnSphereOverlap(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AMainCharacter::StaticClass()))
	{
		OwnerCharacter = Cast<AMainCharacter>(OtherActor);

		if (OwnerCharacter && PickupWidget)
		{
			AFollowCamera* PlayerCamera = OwnerCharacter->GetFollowCamera();
			
			if (PlayerCamera)
			{
				FVector WidgetLocation = PlayerCamera->GetCameraLocation() - PickupWidget->GetComponentLocation();
				FRotator WidgetRotation = WidgetLocation.Rotation();

				PickupWidget->SetWorldRotation(WidgetRotation);
				ShowPickupWidget(true);
				OwnerCharacter->SetOverlappingWeapon(this);
			}
		}
	}
}

void AWeapon::OnEndSphereOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OwnerCharacter && PickupWidget)
	{
		ShowPickupWidget(false);
		OwnerCharacter->SetOverlappingWeapon(nullptr);
		OwnerCharacter = nullptr;
	}
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::ShowPickupWidget(bool bVisible)
{
	PickupWidget->SetVisibility(bVisible);
}

void AWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State;

	switch(WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		ShowPickupWidget(false);
		PickupCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponMesh->SetSimulatePhysics(false);
		WeaponMesh->SetEnableGravity(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EWeaponState::EWS_EquippedSecondary:
		ShowPickupWidget(false);
		PickupCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponMesh->SetSimulatePhysics(false);
		WeaponMesh->SetEnableGravity(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EWeaponState::EWS_Dropped:
		PickupCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	}
}

void AWeapon::Shoot(const FVector& HitTarget)
{
	const USkeletalMeshSocket* MuzzleSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));

	if (!MuzzleSocket) return;
	
	FTransform ProjectileTransform = MuzzleSocket->GetSocketTransform(GetWeaponMesh());

	if (FireAnimation)
	{
		GetWeaponMesh()->PlayAnimation(FireAnimation,false);
	}

	if (MuzzlePartical)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzlePartical, MuzzleSocket->GetSocketTransform(GetWeaponMesh()));
	}

	if (ShootSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSound, GetActorLocation());
	}

	if (ProjectileClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		FVector StartLocation = ProjectileTransform.GetLocation();
		FVector ToTarget = HitTarget - StartLocation;

		FRotator TargetRotation = (bAiming) ? ToTarget.Rotation() : WeaponMesh->GetSocketRotation(FName("MuzzleFlash"));

		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, StartLocation, TargetRotation, SpawnParams);

		MuzzleScatter = DefMuzzleScatter;

		if (bAiming) MuzzleScatter /= AimScatterDivider;

		if (Projectile)
		{
			float DirectionError = FMath::RandRange(-MuzzleScatter, MuzzleScatter);
			FVector LaunchDirection = TargetRotation.Vector() + DirectionError;
				Projectile->FireInDirection(LaunchDirection);
		}

		bAiming = false;
		UE_LOG(LogTemp, Warning, TEXT("Projctile"));

		SpendAmmo();
	}
}

void AWeapon::Dropped()
{
	SetWeaponState(EWeaponState::EWS_Dropped);
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	WeaponMesh->DetachFromComponent(DetachRules);
	SetOwner(nullptr);
	ShowPickupWidget(false);
	
	if (OwnerCharacter)
	{
		OwnerCharacter->SetOverlappingWeapon(nullptr);
		OwnerCharacter = nullptr;
	}

	PlayerController = nullptr;
}

void AWeapon::SpendAmmo()
{
	Ammo = FMath::Clamp(--Ammo, 0, MagCapasity);
}

void AWeapon::AddAmmo(int32 AmmoToAdd)
{
	Ammo = FMath::Clamp(Ammo - AmmoToAdd, 0, MagCapasity);
}

int32 AWeapon::GetRoomInMag()
{
	return MagCapasity - Ammo;
}

bool AWeapon::IsEmpty()
{
	return Ammo <= 0;
}

bool AWeapon::IsFullCapasity()
{
	return Ammo >= MagCapasity;
}

bool AWeapon::IsTargetOwner()
{
	return (WeponTarget == GetOwner()) ? true : false;
}

