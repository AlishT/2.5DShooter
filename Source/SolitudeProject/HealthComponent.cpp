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

	UpdateHUDStates();

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeDamage);
}
void UHealthComponent::UpdateHUDStates()
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
	
	int DeltaDamage = 0;

	if (CurrentArmor < Damage && CurrentArmor != 0)
	{
		DeltaDamage = CurrentArmor - Damage;
		CurrentHealth -= FMath::Abs(DeltaDamage);
	}

	if (CurrentArmor <= 0 && DeltaDamage == 0)
	{
		CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0, MaxHealth);
	}

	CurrentArmor = FMath::Clamp(CurrentArmor - Damage, 0, MaxArmor);

	UpdateHUDStates();

	if (DamageCauser)
	{
		if (BloodPartical)
		{
			FVector ProgectileLocation = DamageCauser->GetActorLocation();
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodPartical, ProgectileLocation);
		}
	}

	OnHealthChanged.Broadcast(CurrentHealth, CurrentArmor);

}

