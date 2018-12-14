// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialInstance.h"
#include "AbilityTypes.generated.h"

class UGameplayAbilityBase;

USTRUCT(BlueprintType)
struct FGameplayAbilityCost
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityInfoCost")
	float Mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityInfoCost")
	float Stamina;

	FGameplayAbilityCost();
	FGameplayAbilityCost(float InMana, float InStamina);
};

USTRUCT(BlueprintType)
struct FGameplayAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityInfo")
	float CooldownDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityInfo")
	FGameplayAbilityCost Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityInfo")
	UMaterialInstance* UIMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityInfo")
	TSubclassOf<UGameplayAbilityBase> AbilityClass;

	FGameplayAbilityInfo();
	FGameplayAbilityInfo(float InCooldownDuration, FGameplayAbilityCost InCost, UMaterialInstance* InUIMaterial, TSubclassOf<UGameplayAbilityBase> InAbilityClass);
};