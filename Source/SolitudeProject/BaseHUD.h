// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

class UCharacterOverlay;
/**
 * 
 */
UCLASS()
class SOLITUDEPROJECT_API ABaseHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void DrawHUD() override;

	UPROPERTY(EditDefaultsOnly, Category = "Player Stats")
	TSubclassOf<class UUserWidget> CharacterOverlayClass;

	UPROPERTY()
	UCharacterOverlay* CharacterOverlay = nullptr;

protected:
	virtual void BeginPlay() override;

	void AddCharacterOverlay();
};
