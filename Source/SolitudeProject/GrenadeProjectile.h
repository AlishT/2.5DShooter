// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "GrenadeProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SOLITUDEPROJECT_API AGrenadeProjectile : public AProjectile
{
	GENERATED_BODY()
	
public:
	AGrenadeProjectile();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float DestroyTimer = 3.f;

	UPROPERTY()
	float CurrentTime = 0.f;

protected:
	virtual void BeginPlay() override;

	void ApplayGrenadeDamage();

	UFUNCTION()
	void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	virtual void Tick(float DeltaTime) override;

};
