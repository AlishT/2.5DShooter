// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "BaseCharacter.h"
#include "BasePlayerController.h"

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

	UpdateHUDState();

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeDamage);
}
void UHealthComponent::UpdateHUDState()
{
	Character = (!Character) ? Cast<ABaseCharacter>(GetOwner()) : Character;

	if (Character)
	{
		PlayerContraller = (!PlayerContraller) ? Character->GetController<ABasePlayerController>() : PlayerContraller;
	}

	if (PlayerContraller)
	{
		PlayerContraller->SetHUDHealth(CurrentHealth, GetMaxHealth());
		PlayerContraller->SetHUDArmor(CurrentArmor, GetMaxArmor());
	}


}

void UHealthComponent::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigateBy, AActor* DamageCauser)
{
	(CurrentArmor > 0) ? CurrentArmor -= Damage : CurrentHealth -= Damage;

	UpdateHUDState();

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

