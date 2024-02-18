// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UParticleSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, Health, float, Armor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOLITUDEPROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

protected:
	
	float MaxHealth = 100.f;

	float CurrentHealth = 0.f;

	float MaxArmor = 100.f;

	float CurrentArmor = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	UParticleSystem* BloodPartical = nullptr;

	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	float GetMaxHealth() const;

	float GetMaxArmor() const;

	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigateBy, AActor* DamageCauser);
};
