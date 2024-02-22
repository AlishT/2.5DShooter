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
