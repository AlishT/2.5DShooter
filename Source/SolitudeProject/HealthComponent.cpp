// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CurrentHealth = GetMaxHealth();
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeDamage);
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UHealthComponent::GetMaxArmor() const
{
	return CurrentArmor;
}

void UHealthComponent::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigateBy, AActor* DamageCauser)
{
	(CurrentArmor > 0) ? CurrentArmor -= Damage : CurrentHealth -= Damage;

	if (DamagedActor)
	{
		if (BloodPartical)
		{
			FVector ProgectileLocation = DamagedActor->GetActorLocation();
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodPartical, ProgectileLocation);
		}
	}

	OnHealthChanged.Broadcast(CurrentHealth, CurrentArmor);

}

