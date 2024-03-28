// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileRocket.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class SOLITUDEPROJECT_API AProjectileRocket : public AProjectile
{
	GENERATED_BODY()

private:
	//UPROPERTY(EditDefaultsOnly, Category = "Default")
	//UNiagaraSystem* TrailSystem = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	USoundCue* Rockedloop = nullptr;

	UPROPERTY(Transient)
	UAudioComponent* RockedLoopComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	USoundAttenuation* LoopingSoundAttenuation = nullptr;

	//UPROPERTY(EditDefaultsOnly, Category = "Default")
	//UNiagaraSystem* Expolosion;

protected:
	virtual void BeginPlay() override;

	//UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
