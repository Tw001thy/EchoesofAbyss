// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	WeaponRoot = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));
	SetRootComponent(WeaponRoot);

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::Fire(const FVector& TraceStart, const FVector& TraceDirection)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	
	const FVector ShotDirection = TraceDirection.GetSafeNormal();
	if (ShotDirection.IsNearlyZero())
	{
		return;
	}
	
	const FVector TraceEnd = TraceStart + ShotDirection * MaxTraceDistance;
	
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(WeaponCameraTrace), false);
	
	// 忽略武器 Actor
	QueryParams.AddIgnoredActor(this);
	
	// 忽略武器拥有者
	if (AActor* WeaponOwner = GetOwner())
	{
		QueryParams.AddIgnoredActor(WeaponOwner);
	}
	
	if (APawn* InstigatorPawn = GetInstigator())
	{
		QueryParams.AddIgnoredActor(InstigatorPawn);
	}
	
	FHitResult Hit;
	const bool bHit = World->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
	
#if ENABLE_DRAW_DEBUG
	if (bDrawDebugTrace)
	{
		const FVector Debugend = bHit ? Hit.ImpactPoint : TraceEnd;
		const FColor DebugColor = bHit ? FColor::Green : FColor::Red;
		
		DrawDebugLine(World, TraceStart, Debugend, DebugColor, false, 1.0f, 0, 1.5f);
		
		if (bHit)
		{
			DrawDebugPoint(
				World,
				Hit.ImpactPoint,
				8.0f,
				FColor::Yellow,
				false,
				1.0f
			);
		}
	}
#endif
	
	if (!bHit)
	{
		return;
	}
	
	AActor* HitActor = Hit.GetActor();
	
	if (!HitActor || !HitActor->CanBeDamaged())
	{
		return;
	}
	
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	
	FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
	
	HitActor->TakeDamage(Damage, DamageEvent, OwnerPawn ? OwnerPawn->GetController() : nullptr, this);
}







