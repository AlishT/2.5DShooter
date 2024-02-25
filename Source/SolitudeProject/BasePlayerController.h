// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class ABaseHUD;
/**
 * 
 */
UCLASS()
class SOLITUDEPROJECT_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDArmor(float Armor, float MaxArmor);
	void SetHUDWeaponAmmo(int32 Ammo);
	void SetHUDCarriedAmmo(int32 Ammo);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	ABaseHUD* BaseHUD = nullptr;
};
