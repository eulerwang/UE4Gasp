// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GATargetActorGroundSelect.generated.h"

class UDecalComponent;
class USceneComponent;

/**
 * 
 */
UCLASS()
class UE4GASP_API AGATargetActorGroundSelect : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

	AGATargetActorGroundSelect();
	
public:
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "GroundSelect")
	FVector GetPlayerLookingPoint();

	UPROPERTY(BlueprintReadWrite, Category = "GroundSelect", meta = ( ExposeOnSpawn = true ))
	float Radius;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GroundSelect")
	UDecalComponent* Decal;

	USceneComponent* RootComponent;
};
