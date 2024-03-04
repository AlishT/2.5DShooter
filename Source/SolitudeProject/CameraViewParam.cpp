// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraViewParam.h"
#include "Components/BoxComponent.h"
#include "CameraParamInterface.h"
#include "MainCharacter.h"
#include "FollowCamera.h"

// Sets default values
ACameraViewParam::ACameraViewParam()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(GetRootComponent());

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACameraViewParam::OnBeginBoxOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACameraViewParam::OnEndBoxOverlap);
}

// Called when the game starts or when spawned
void ACameraViewParam::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACameraViewParam::OnBeginBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	if (OtherActor->IsA(AMainCharacter::StaticClass()))
	{
		Character = Cast<AMainCharacter>(OtherActor);

		ICameraParamInterface* CameraParamInterface = Cast<ICameraParamInterface>(Character->GetFollowCamera());
		
		if (CameraParamInterface)
		{
			CameraParamInterface->Execute_SetCameraNewParam(Character->GetFollowCamera(), NewArmLenght, NewCameraLocationZ, NewCameraOffset);
		}
	}
}

void ACameraViewParam::OnEndBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Character && OtherActor->IsA(AMainCharacter::StaticClass()))
	{
		ICameraParamInterface* CameraParamInterface = Cast<ICameraParamInterface>(Character->GetFollowCamera());

		if (CameraParamInterface)
		{
			CameraParamInterface->Execute_SetCameraNewParam(Character->GetFollowCamera(), DefaultArmLenght, DefaultCameraLocationZ, DefaultOffset);

			Character = nullptr;
		}
	}
}

// Called every frame
void ACameraViewParam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

