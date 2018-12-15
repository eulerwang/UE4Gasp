// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GATargetActorEnemiesAround.generated.h"

/**
 * 
 */
UCLASS()
class UE4GASP_API AGATargetActorEnemiesAround : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	void StartTargeting(UGameplayAbility* Ability) override;
	void ConfirmTargetingAndContinue() override;

	UPROPERTY(BlueprintReadWrite, Category = "FindAround", meta = (ExposeOnSpawn = true))
	float Radius;
};
