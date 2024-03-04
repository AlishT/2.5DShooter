// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraViewParam.generated.h"

class UBoxComponent;
class AMainCharacter;

UCLASS()
class SOLITUDEPROJECT_API ACameraViewParam : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraViewParam();

public:
	UPROPERTY(EditAnywhere, Category = "Default")
	UBoxComponent* CollisionBox = nullptr;

	UPROPERTY(Transient)
	AMainCharacter* Character = nullptr;

	UPROPERTY(EditAnywhere, Category = "Default")
	float NewArmLenght = 0.f;

	UPROPERTY(EditAnywhere, Category = "Default")
	float NewCameraLocationZ = 0.f;

	UPROPERTY(EditAnywhere, Category = "Default")
	float NewCameraOffset = 0.f;

	float DefaultArmLenght = 900.f;

	float DefaultCameraLocationZ = 150.f;

	float DefaultOffset = 25;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginBoxOverlap(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndBoxOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
