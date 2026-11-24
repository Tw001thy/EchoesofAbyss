// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USceneComponent;

UCLASS()
class ECHOESOFABYSS_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();
	
	// 由玩家角色传入第一人称相机的位置和屏幕中心方向
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire(const FVector& TraceStart, const FVector& TraceDirection);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> WeaponRoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Damage = 20.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float MaxTraceDistance = 10000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bDrawDebugTrace = true;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
