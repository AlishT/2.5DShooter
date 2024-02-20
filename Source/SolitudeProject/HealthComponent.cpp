// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "BaseCharacter.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = GetMaxHealth();
	CurrentArmor = GetMaxArmor();

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeDamage);
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

		ABaseCharacter* Character = Cast<ABaseCharacter>(DamagedActor);
		
		Character->PlayHitReactMontage();
	}

	OnHealthChanged.Broadcast(CurrentHealth, CurrentArmor);

}

