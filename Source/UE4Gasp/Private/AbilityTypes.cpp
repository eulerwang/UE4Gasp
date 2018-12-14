// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityTypes.h"

FGameplayAbilityCost::FGameplayAbilityCost() :
	Mana(0.0f),
	Stamina(0.0f)
{

}

FGameplayAbilityCost::FGameplayAbilityCost(float InMana, float InStamina) :
	Mana(InMana),
	Stamina(InStamina)
{

}


FGameplayAbilityInfo::FGameplayAbilityInfo() :
	CooldownDuration(0.0f),
	Cost(FGameplayAbilityCost(0.0f, 0.0f)),
	UIMaterial(nullptr),
	AbilityClass(nullptr)
{

}

FGameplayAbilityInfo::FGameplayAbilityInfo(float InCooldownDuration, FGameplayAbilityCost InCost, UMaterialInstance* InUIMaterial, TSubclassOf<UGameplayAbilityBase> InAbilityClass) :
	CooldownDuration(InCooldownDuration),
	Cost(InCost),
	UIMaterial(InUIMaterial),
	AbilityClass(InAbilityClass)
{

}