// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileRocket.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundAttenuation.h"
#include "Sound/SoundCue.h"
//#include "NiagaraSystem.h"
//#include "NiagaraComponentPool.h"

void AProjectileRocket::BeginPlay()
{
	Super::BeginPlay();

	if (Rockedloop && LoopingSoundAttenuation)
	{
		RockedLoopComponent = UGameplayStatics::SpawnSoundAttached(Rockedloop, GetRootComponent(), FName(), GetActorLocation(), EAttachLocation::KeepWorldPosition, false, 1.f, 1.f, 0.f, LoopingSoundAttenuation, (USoundConcurrency*)nullptr, false);
	}
}

void AProjectileRocket::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	APawn* Pawn = GetInstigator();
	//	
	if (Pawn)
	{
		AController* OwnerInstigator = Pawn->GetInstigatorController();
		UClass* DamageTypeClass = UDamageType::StaticClass();

		UGameplayStatics::ApplyRadialDamageWithFalloff(this, Damage, MinDamage, GetActorLocation(), DamageInnerRadius, DamageOuterRadius, 1.f, DamageTypeClass, TArray<AActor*>(), this, OwnerInstigator);
	}

	if (Rockedloop && RockedLoopComponent->IsPlaying())
	{
		RockedLoopComponent->Stop();
	}
	//UNiagaraFunctionLibrary::SpawnSystemAttached(Expolosion, this->RootComponent, "NAME_None", GetActorLocation(), GetActorRotation(), EAttachLocation::Type::KeepRelativeOffset, true, true);

	Super::OnHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}
