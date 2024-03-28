// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "BaseCharacter.h"
#include "SolitudeProject.h"
#include "NiagaraFunctionLibrary.h"

AGrenadeProjectile::AGrenadeProjectile()
{
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->ProjectileGravityScale = 1.0f;
}

void AGrenadeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentTime += DeltaTime;

	if (CurrentTime >= DestroyTimer)
	{
		ApplayGrenadeDamage();
	}
}

void AGrenadeProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovement->OnProjectileBounce.AddDynamic(this, &AGrenadeProjectile::OnBounce);
}

void AGrenadeProjectile::ApplayGrenadeDamage()
{
	APawn* Pawn = GetInstigator();
	
	if (Pawn)
	{
		AController* OwnerInstigator = Pawn->GetInstigatorController();
		UClass* DamageTypeClass = UDamageType::StaticClass();

		UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), Damage, MinDamage, GetActorLocation(), DamageInnerRadius, DamageOuterRadius, 1.f, DamageTypeClass, TArray<AActor*>(), this, OwnerInstigator);

		DestroyProjectile();
	}
}

void AGrenadeProjectile::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{

}

void AGrenadeProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}

