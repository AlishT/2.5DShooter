// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CombatComponent.h"
#include "Weapon.h"
#include "BasePlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "FollowCamera.h"

AMainCharacter::AMainCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 1000.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f; 
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	ABasePlayerController* PlayerController = GetController<ABasePlayerController>();

	if (!PlayerController) return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	if (Subsystem)
	{
		Subsystem->AddMappingContext(MovingMC, 0);
	}

	PlayerController->bShowMouseCursor = true;

	if (FollowCameraClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		float CameraYaw = -90;
		FollowCamera = GetWorld()->SpawnActor<AFollowCamera>(FollowCameraClass, GetActorLocation(), FRotator(0.f, CameraYaw, 0.f), SpawnParams);

		PlayerController->SetViewTargetWithBlend(FollowCamera);
	}
		PlayerController->bShowMouseCursor = true;

}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//bSprinting = false;

	if (FollowCamera)
	{
		FollowCamera->UpdateCameraPos(GetActorLocation());
	}

	FHitResult HitRes = FHitResult();
	
	bool Hit = GetController<APlayerController>()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, false, HitRes);
	
	if (Hit)
	{
		if (!HitRes.bBlockingHit) return;
		
		//if(HitRes.GetActor() == this) return;
	
		TargetLocation = HitRes.ImpactPoint;
	
		RotateCharacter();
	
	
	//AimOffset(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("AO Yaw %f"), AO_Yaw);
	UE_LOG(LogTemp, Warning, TEXT("AO Pitch %f"), AO_Pitch);

	if (CombatComponent)
	{
		CombatComponent->SetAiming(false);
		
		if (CombatComponent->GetEquippedWeapon())
		{
			CombatComponent->GetEquippedWeapon()->SetTarget(HitRes.GetActor());
		}
	}
	}
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (PlayerEnhancedInputComponent)
	{
		PlayerEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		PlayerEnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
		PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMainCharacter::ToJump);
		PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMainCharacter::StopJumping);
		PlayerEnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AMainCharacter::Crouching);
		PlayerEnhancedInputComponent->BindAction(WeaponEquipAction, ETriggerEvent::Triggered, this, &AMainCharacter::EquipWeapon);
		PlayerEnhancedInputComponent->BindAction(DroppedWeaponAction, ETriggerEvent::Triggered, this, &AMainCharacter::DroppedWeapon);
		PlayerEnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AMainCharacter::Fire);
		PlayerEnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AMainCharacter::Aiming);
		PlayerEnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AMainCharacter::Sprint);
		PlayerEnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AMainCharacter::Reload);
	}
}

void AMainCharacter::Move(const FInputActionValue& Value)
{
	float DirectionValue = Value.Get<float>();

	if (Controller && DirectionValue != 0) 
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		//FVector Forward = FVector::ForwardVector * DirectionValue;
		AddMovementInput(Direction, DirectionValue);

		if (!IsAiming())
		{
			FollowCamera->SetCamDirection(DirectionValue);
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Direction Value %f"), DirectionValue);
	}
}

void AMainCharacter::Look(const FInputActionValue& Value)
{
	/*FVector2D Direction = Value.Get<FVector2D>();

	AddControllerYawInput(Direction.X);
	AddControllerPitchInput(Direction.Y);*/
}


void AMainCharacter::Crouching()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch(); 
	}
}

void AMainCharacter::EquipWeapon()
{
	if (CombatComponent)
	{
		CombatComponent->EquipWeapon(PickapedWeapom);
	}

	//bEquipWeapon = bEquip;
}

void AMainCharacter::DroppedWeapon()
{
	if (CombatComponent && CombatComponent->GetEquippedWeapon())
	{
		CombatComponent->GetEquippedWeapon()->Dropped();
	}
}

void AMainCharacter::Fire()
{
	if (IsWeaponEquipeed())
	{
		CombatComponent->Fire();
	}
}

void AMainCharacter::Aiming()
{
	if (CombatComponent)
	{
		CombatComponent->SetAiming(true);
		if (!CombatComponent->GetEquippedWeapon()) return;
		CombatComponent->GetEquippedWeapon()->bAiming = true;
	}
}

void AMainCharacter::Sprint()
{
	if (IsAiming()) return;
	bSprinting = false;
}

void AMainCharacter::ToJump()
{
	if (IsAiming()) return;
	Jump();
}

void AMainCharacter::Reload()
{
	if (CombatComponent) 
	{
		CombatComponent->Reload();
	}
}

void AMainCharacter::RotateCharacter()
{
	FVector Direction = TargetLocation - GetActorLocation();

	FRotator Rotation = Direction.Rotation();
	Rotation.Normalize();

	AO_Pitch = Rotation.Pitch;
	if (IsAiming())
	{
		SetActorRotation(FRotator(0.f, Rotation.Yaw, 0.f));
	}
}
