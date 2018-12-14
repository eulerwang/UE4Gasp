// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AttributeSetBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegateOnHealthChange, float, Health, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegateOnManaChange, float, Mana, float, MaxMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegateOnStaminaChange, float, Stamina, float, MaxStamina);

/**
 * Base Attributes Stats.
 */
UCLASS()
class UE4GASP_API UAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()


public:
	UAttributeSetBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
	FGameplayAttributeData Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
	FGameplayAttributeData Mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
	FGameplayAttributeData MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
	FGameplayAttributeData Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
	FGameplayAttributeData MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
	FGameplayAttributeData MoveSpeed;

	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data);

	FDelegateOnHealthChange OnHealthChange;
	FDelegateOnManaChange OnManaChange;
	FDelegateOnStaminaChange OnStaminaChange;
};
