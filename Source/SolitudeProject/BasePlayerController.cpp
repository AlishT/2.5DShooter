// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"
#include "SolitudeGameMode.h"
#include "BaseHUD.h"
#include "CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	BaseHUD = GetHUD<ABaseHUD>();
}

void ABasePlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	BaseHUD = (!BaseHUD) ? GetHUD<ABaseHUD>() : BaseHUD;

	bool BHUDValid = BaseHUD && BaseHUD->CharacterOverlay && BaseHUD->CharacterOverlay->HealthBar && BaseHUD->CharacterOverlay->HealthText;
	
	if (BHUDValid)
	{
		const float HealthPercetnt = Health / MaxHealth;
		BaseHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercetnt);
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		BaseHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
}

void ABasePlayerController::SetHUDArmor(float Armor, float MaxArmor)
{
	BaseHUD = (!BaseHUD) ? GetHUD<ABaseHUD>() : BaseHUD;

	bool BHUDValid = BaseHUD && BaseHUD->CharacterOverlay && BaseHUD->CharacterOverlay->ArmorBar && BaseHUD->CharacterOverlay->ArmorText;

	if (BHUDValid)
	{
		const float ArmorPercetnt = Armor / MaxArmor;
		BaseHUD->CharacterOverlay->ArmorBar->SetPercent(ArmorPercetnt);
		FString ArmorText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Armor), FMath::CeilToInt(MaxArmor));
		BaseHUD->CharacterOverlay->ArmorText->SetText(FText::FromString(ArmorText));
	}
}

void ABasePlayerController::SetHUDWeaponAmmo(int32 Ammo)
{
	BaseHUD = (!BaseHUD) ? GetHUD<ABaseHUD>() : BaseHUD;

	bool BHUDValid = BaseHUD && BaseHUD->CharacterOverlay && BaseHUD->CharacterOverlay->AmmoCount;

	if (BHUDValid)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		BaseHUD->CharacterOverlay->AmmoCount->SetText(FText::FromString(AmmoText));
	}
}

void ABasePlayerController::SetHUDCarriedAmmo(int32 Ammo)
{
	BaseHUD = (!BaseHUD) ? GetHUD<ABaseHUD>() : BaseHUD;

	bool BHUDValid = BaseHUD && BaseHUD->CharacterOverlay && BaseHUD->CharacterOverlay->CarriedAmmo;

	if (BHUDValid)
	{
		FString CarriedAmmoText = FString::Printf(TEXT("%d"), Ammo);
		BaseHUD->CharacterOverlay->CarriedAmmo->SetText(FText::FromString(CarriedAmmoText));
	}
}
