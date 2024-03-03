// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;
class UParticleSystem;
class USoundCue;

UCLASS()
class SOLITUDEPROJECT_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Defauult")
	UStaticMeshComponent* ProjectileMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defauult")
	UBoxComponent* CollisionBox = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defauult")
	UProjectileMovementComponent* ProjectileMovement = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Defauult")
	float Damage = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Defauult")
	UParticleSystem* Tracer;

	UParticleSystemComponent* TracerComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Defauult")
	UParticleSystem* ImpactPartical = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Defauult")
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
