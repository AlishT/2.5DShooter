// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponTypes.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),

	EWS_MAX UMETA(DispalyName = "DefaultMAX")

};

class USkeletalMeshComponent;
class USphereComponent;
class UWidgetComponent;
class AProjectile;
class UParticleSystem;
class USoundCue;
class AMainCharacter;
class ABasePlayerController;

UCLASS()
class SOLITUDEPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USphereComponent* PickupCollision = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UWidgetComponent* PickupWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	class UAnimationAsset* FireAnimation = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	UParticleSystem* MuzzlePartical = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	USoundCue* ShootSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	float MuzzleScatter = 0.f;

	int32 Ammo = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	int32 MagCapasity = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	EWeaponType WeaponType;

	UPROPERTY()
	AMainCharacter* OwnerCharacter = nullptr;

	UPROPERTY()
	ABasePlayerController* OwnerPlayerController = nullptr;

	UPROPERTY()
	AActor* WeponTarget = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEndSphereOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	bool bAiming = false;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	bool bAutomatic = true;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	float FireDelay = 0.f;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ShowPickupWidget(bool bVisible);

	void SetWeaponState(EWeaponState State);

	void Shoot(const FVector& HitTarget);

	void Dropped();

	void SpendAmmo();

	bool IsEmpty();

	bool IsTargetOwner();

	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE void SetTarget(AActor* Target) { WeponTarget = Target; }

	//FORCEINLINE bool IsPistol() const { return bPistol; }

};
