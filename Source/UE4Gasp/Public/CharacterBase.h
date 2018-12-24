// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilityBase.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Engine/EngineTypes.h"
#include "CharacterBase.generated.h"

class UAttributeSetBase;

UCLASS()
class UE4GASP_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	UAttributeSetBase* AttributeSetBaseComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	FGameplayTag FullHealthTag;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	// TArray<TSubclassOf<UGameplayAbility>> AbilitiesToAcquire;

	// Gives Ability to Character.
	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	void AcquireAbility(TSubclassOf<UGameplayAbility> AbilityToAcquire);

	// Gives All abilities to Character
	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	void AcquireAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToAcquire);

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	void AddGameplayTag(FGameplayTag& TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	void RemoveGameplayTag(FGameplayTag& TagToRemove);

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	bool IsOtherActorHostile(ACharacterBase* OtherActor) const;

	// Called from RPGAttributeSet, these call BP events Below
	void HandleHealthChanged(float Health, float MaxHealth);
	void HandleManaChanged(float Mana, float MaxMana);
	void HandleStaminaChanged(float Stamina, float MaxStamina);
	void HandleMoveSpeedChanged(float MoveSpeed);
	void HandleDamage(float Damage);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnHealthChanged"))
	void BP_OnHealthChanged(float Health, float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnManaChanged"))
	void BP_OnManaChanged(float Mana, float MaxMana);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnStaminaChanged"))
	void BP_OnStaminaChanged(float Stamina, float MaxStamina);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnMoveSpeedChanged"))
	void BP_OnMoveSpeedChanged(float MoveSpeed);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnDamaged"))
	void BP_OnDamaged(float Damage);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnDie"))
	void BP_OnDie();

	// Called when Stamina reach 0
	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnStaminaDrained"))
	void BP_OnStaminaDrained();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	void HitStun(float StunDuration);

	uint8 GetTeamID() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Implement AbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

private:
	bool bIsDead;
	bool bIsStun;
	uint8 TeamID;

	// Determine TeamId based on the type of controller
	void AutoDetermineTeamIdByController();

	FTimerHandle StunTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void DisableInputControl();
	void EnableInputControl();
	void AddAbilityToUI(TSubclassOf<UGameplayAbilityBase> AbilityToAdd);
};
