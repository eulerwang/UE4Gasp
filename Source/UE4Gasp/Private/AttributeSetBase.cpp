// Fill out your copyright notice in the Description page of Project Settings.

#include "AttributeSetBase.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"
#include "CharacterBase.h"
#include "GameFramework/PlayerController.h"

UAttributeSetBase::UAttributeSetBase()
	: Health(200.0f),
	MaxHealth(200.0f),
	Mana(100.0f),
	MaxMana(150.0f),
	Stamina(150.0f),
	MaxStamina(150.0f),
	MoveSpeed(1.0f)
{

}

void UAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData &Data)
{
	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	APlayerController* TargetController = nullptr;
	ACharacterBase* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<ACharacterBase>(TargetActor);
	}

	// Health
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<UProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Health)))
	{
		float ClampedHealth = FMath::Clamp(Health.GetCurrentValue(), 0.0f, MaxHealth.GetCurrentValue());
		Health.SetBaseValue(ClampedHealth);
		Health.SetCurrentValue(ClampedHealth);

		UE_LOG(LogTemp, Warning, TEXT("Health changed, magnitude: %f"), Data.EvaluatedData.Magnitude);
		
		if (TargetCharacter)
		{
			if (Health.GetCurrentValue() == MaxHealth.GetCurrentValue())
			{
				TargetCharacter->AddGameplayTag(TargetCharacter->FullHealthTag);
			}
			else
			{
				TargetCharacter->RemoveGameplayTag(TargetCharacter->FullHealthTag);
			}

			TargetCharacter->HandleHealthChanged(Health.GetCurrentValue(), MaxHealth.GetCurrentValue());

			if (Data.EvaluatedData.Magnitude < 0)
			{
				TargetCharacter->HandleDamage(Data.EvaluatedData.Magnitude);
			}
		}
	}

	// Mana
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<UProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Mana)))
	{
		float ClampedMana = FMath::Clamp(Mana.GetCurrentValue(), 0.0f, MaxMana.GetCurrentValue());
		Mana.SetBaseValue(ClampedMana);
		Mana.SetCurrentValue(ClampedMana);
		if (TargetCharacter)
		{
			TargetCharacter->HandleManaChanged(Mana.GetCurrentValue(), MaxMana.GetCurrentValue());
		}
	}

	// Stamina
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<UProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Stamina)))
	{
		float ClampedStamina = FMath::Clamp(Stamina.GetCurrentValue(), 0.0f, MaxStamina.GetCurrentValue());
		Stamina.SetBaseValue(ClampedStamina);
		Stamina.SetCurrentValue(ClampedStamina);
		if (TargetCharacter)
		{
			TargetCharacter->HandleStaminaChanged(Stamina.GetCurrentValue(), MaxStamina.GetCurrentValue());
		}
	}

	// MoveSpeed
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<UProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, MoveSpeed)))
	{
		if (TargetCharacter)
		{
			TargetCharacter->HandleMoveSpeedChanged(MoveSpeed.GetCurrentValue());
		}
	}
}