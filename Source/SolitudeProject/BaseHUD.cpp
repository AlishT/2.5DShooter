// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHUD.h"
#include "GameFramework/PlayerController.h"
#include "CharacterOverlay.h"
#include "Blueprint/UserWidget.h"

void ABaseHUD::DrawHUD()
{
	Super::DrawHUD();


}

void ABaseHUD::BeginPlay()
{
	Super::BeginPlay();

	AddCharacterOverlay();
}

void ABaseHUD::AddCharacterOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && CharacterOverlayClass)
	{
		CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
	}
}

