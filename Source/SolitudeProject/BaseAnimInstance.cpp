// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAnimInstance.h"
#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon.h"

void UBaseAnimInstance::NativeInitializeAnimation()
{
	BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
}

void UBaseAnimInstance::NativeUpdateAnimation(float DealtaTime)
{
	Super::NativeUpdateAnimation(DealtaTime);

	if (!BaseCharacter)
	{
		BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
	}

	if (BaseCharacter)
	{
		//Get the lateral speed of the character from velocity
		FVector Velocity = BaseCharacter->GetVelocity();
		Velocity.Z = 0.f;
		//Velocity.Y = 0.f;
		Speed = Velocity.Size();

		bIsSprinting = BaseCharacter->bSprinting;

		//Is the character in the air?
		bIsInAir = BaseCharacter->GetCharacterMovement()->IsFalling();
		
		//Ser Character Direction
		FRotator BaseRotation = BaseCharacter->GetActorRotation();
		Direction = CalculateDirection(Velocity, BaseRotation);

		//Is the character accelerating?
		float Acceleration = BaseCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size();
		bIsAccelerating = (Acceleration > 0) ? true : false;

		//Set movemntOffset
		FRotator AimRotetion = BaseCharacter->GetBaseAimRotation();
		FRotator MovemetnRotation = UKismetMathLibrary::MakeRotFromX(BaseCharacter->GetVelocity());
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovemetnRotation, AimRotetion).Pitch;

		bWeaponEquipped = BaseCharacter->IsWeaponEquipeed();

		bIsCrouched = BaseCharacter->bIsCrouched;
		bIsAming = BaseCharacter->IsAiming();

		AO_Yaw = BaseCharacter->GetAOYaw();
		AO_Pitch = BaseCharacter->GetAOPitch();

		EquippedWeapon = BaseCharacter->GetEquippedWeapon();

		if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && BaseCharacter->GetMesh()) 
		{
			//FABRIK
			LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocked"), ERelativeTransformSpace::RTS_World);
			FVector OutPosition;
			FRotator OutRotation;
			BaseCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
			LeftHandTransform.SetLocation(OutPosition);
			LeftHandTransform.SetRotation(FQuat(OutRotation));
		}
	}
}
