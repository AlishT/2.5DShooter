#include "FollowCamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "MainCharacter.h"
#include "CameraParamInterface.h"

AFollowCamera::AFollowCamera()
{
	PrimaryActorTick.bCanEverTick = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bDoCollisionTest = false;
	//CameraBoom->SocketOffset = FVector(0.f, 250.f, 0.f);

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	PlayerCamera->bUsePawnControlRotation = false;
}

void AFollowCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFollowCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFollowCamera::UpdateCameraPos(const FVector& FollowTargetLocation)
{
	FVector Location = FMath::VInterpTo(GetActorLocation(), FollowTargetLocation, CameraDeltaTime, InterpSpeed);

	//Location.Z = GetActorLocation().Z;

	FVector Offset = FollowTargetLocation.ForwardVector * CamOffset;

	FVector UpdatedLocation = Location + Offset * CameraDirection;

	SetActorLocation(UpdatedLocation);
}

void AFollowCamera::SetCameraNewParam_Implementation(float ArmLenght, float LocationZ, float Offset)
{
	CameraBoom->TargetArmLength = ArmLenght;

	CamOffset = Offset; 

	FVector Location = FVector(0.f, 0.f, LocationZ);

	PlayerCamera->SetRelativeLocation(Location);

}

