// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileRocket.generated.h"

/**
 * 
 */
UCLASS()
class SOLITUDEPROJECT_API AProjectileRocket : public AProjectile
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Defauult")
	float MinDamage = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Defauult")
	float DamageInnerRadius = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Defauult")
	float DamageOuterRadius = 200.f;

protected:
	//UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
