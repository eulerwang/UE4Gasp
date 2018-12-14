// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayAbilityBase.h"
#include "AttributeSetBase.h"

FGameplayAbilityInfo UGameplayAbilityBase::GetAbilityInfo()
{
	UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect();
	UGameplayEffect* CostEffect = GetCostGameplayEffect();
	if (CooldownEffect && CostEffect)
	{
		float CooldownDuration = 0;
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(1, CooldownDuration);

		FGameplayAbilityCost AbilityCost = FGameplayAbilityCost();
		if (CostEffect->Modifiers.Num() > 0)
		{
			for (FGameplayModifierInfo ModifierInfo : CostEffect->Modifiers)
			{
				// If Mana
				if (ModifierInfo.Attribute.GetUProperty() == FindFieldChecked<UProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Mana)))
				{
					float ManaCost = 0.0f;
					ModifierInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(1, ManaCost);
					AbilityCost.Mana = ManaCost;
				}

				// If Stamina
				if (ModifierInfo.Attribute.GetUProperty() == FindFieldChecked<UProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Stamina)))
				{
					float StaminaCost = 0.0f;
					ModifierInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(1, StaminaCost);
					AbilityCost.Stamina = StaminaCost;

				}
			}

			return FGameplayAbilityInfo(CooldownDuration, AbilityCost, UIMaterial, GetClass());
		}
	}

	return FGameplayAbilityInfo();
}
