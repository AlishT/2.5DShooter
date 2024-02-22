// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FollowCamera.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AMainCharacter;

UCLASS()
class SOLITUDEPROJECT_API AFollowCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFollowCamera();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	USpringArmComponent* CameraBoom = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	UCameraComponent* PlayerCamera = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float InterpSpeed = 3.f;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float CamOffset = 0.f;

	float CameraDeltaTime = 0.02f;

	float CameraDirection = 0.f;

protected:
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateCameraPos(const FVector& FollowTargetLocation);

	FORCEINLINE void SetCamDirection(const float CamDirection) { CameraDirection = CamDirection; }
};
