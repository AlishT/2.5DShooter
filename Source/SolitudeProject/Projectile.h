// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;
class UParticleSystem;
class USoundCue;
class UNiagaraSystem;

UCLASS()
class SOLITUDEPROJECT_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	UStaticMeshComponent* ProjectileMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	UBoxComponent* CollisionBox = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	UProjectileMovementComponent* ProjectileMovement = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float Damage = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float MinDamage = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float DamageInnerRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float DamageOuterRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	UParticleSystem* Tracer = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	UNiagaraSystem* TrailSystem = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	UNiagaraSystem* TracerSystem = nullptr;

	UPROPERTY(Transient)
	UParticleSystemComponent* TracerComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	UParticleSystem* ImpactPartical = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	USoundCue* ImpactSound = nullptr;

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FireInDirection(const FVector& ShootDirection);

	void DestroyProjectile();
};
