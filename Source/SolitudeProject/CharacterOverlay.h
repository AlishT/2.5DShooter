// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterOverlay.generated.h"

/**
 * 
 */
UCLASS()
class SOLITUDEPROJECT_API UCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Meta = (BindWidget))
	class UProgressBar* HealthBar = nullptr;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* HealthText = nullptr;

	UPROPERTY(Meta = (BindWidget))
	class UProgressBar* ArmorBar = nullptr;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* ArmorText = nullptr;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* AmmoCount = nullptr;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* CarriedAmmo = nullptr;

};
