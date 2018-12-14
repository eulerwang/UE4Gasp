// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include "AttributeSetBase.h"
#include "PlayerControllerBase.h"
#include "AbilityTypes.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "TimerManager.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup components
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AttributeSetBaseComponent = CreateDefaultSubobject<UAttributeSetBase>("AttributeSetBaseComponent");
	bIsDead = false;
	bIsStun = false;
	TeamID = 255;
}

void ACharacterBase::AutoDetermineTeamIdByController()
{
	AController* Controller = GetController();
	if (Controller && Controller->IsPlayerController())
	{
		TeamID = 0;
	}
	else if (Controller && Controller->GetPawn() && Controller->GetPawn()->AIControllerClass != nullptr)
	{
		TeamID = 1;
	}
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	AttributeSetBaseComponent->OnHealthChange.AddDynamic(this, &ACharacterBase::OnHealthChanged);
	AttributeSetBaseComponent->OnManaChange.AddDynamic(this, &ACharacterBase::OnManaChanged);
	AttributeSetBaseComponent->OnStaminaChange.AddDynamic(this, &ACharacterBase::OnStaminaChanged);
	AutoDetermineTeamIdByController();

	// Setup Full Health Tag
	if (AttributeSetBaseComponent->Health.GetCurrentValue() == AttributeSetBaseComponent->MaxHealth.GetCurrentValue())
	{
		UE_LOG(LogTemp, Warning, TEXT("Adding FullHealtGameplayTag"));
		AddGameplayTag(FullHealthTag);
	}
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACharacterBase::AcquireAbility(TSubclassOf<UGameplayAbility> AbilityToAcquire)
{
	if (!AbilitySystemComponent) 
	{
		return;
	}

	if (HasAuthority() && AbilityToAcquire)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityToAcquire, 1, 0));
	}

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void ACharacterBase::AcquireAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToAcquire)
{
	for (TSubclassOf<UGameplayAbility> Ability : AbilitiesToAcquire)
	{
		AcquireAbility(Ability);
		if (Ability->IsChildOf(UGameplayAbilityBase::StaticClass()))
		{
			TSubclassOf<UGameplayAbilityBase> AbilityBaseClass = *Ability;
			if (AbilityBaseClass)
			{
				AddAbilityToUI(AbilityBaseClass);
			}
		}
	}
}

void ACharacterBase::AddGameplayTag(FGameplayTag& TagToAdd)
{
	GetAbilitySystemComponent()->AddLooseGameplayTag(TagToAdd);
	GetAbilitySystemComponent()->SetTagMapCount(TagToAdd, 1);
}

void ACharacterBase::RemoveGameplayTag(FGameplayTag& TagToRemove)
{
	GetAbilitySystemComponent()->RemoveLooseGameplayTag(TagToRemove);
}

void ACharacterBase::OnHealthChanged(float Health, float MaxHealth)
{
	if (Health <= 0 && !bIsDead)
	{
		bIsDead = true;
		BP_OnDie();
	}

	BP_OnHealthChanged(Health, MaxHealth);
}

void ACharacterBase::OnManaChanged(float Mana, float MaxMana)
{
	BP_OnManaChanged(Mana, MaxMana);
}

void ACharacterBase::OnStaminaChanged(float Stamina, float MaxStamina)
{
	if (Stamina <= 0)
	{
		BP_OnStaminaDrained();
	}

	BP_OnStaminaChanged(Stamina, MaxStamina);
}

void ACharacterBase::OnMoveSpeedChanged(float MoveSpeed)
{
	UE_LOG(LogTemp, Warning, TEXT("OnMoveSpeedChanged, change MaxWalkSpped to: %f"), MoveSpeed);
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	BP_OnMoveSpeedChanged(MoveSpeed);
}

bool ACharacterBase::IsOtherActorHostile(ACharacterBase* OtherActor) const
{
	return TeamID != OtherActor->GetTeamID();
}

void ACharacterBase::HitStun(float StunDuration)
{
	if (bIsStun)
	{
		return;
	}

	bIsStun = true;
	DisableInputControl();
	GetWorldTimerManager().SetTimer(StunTimerHandle, this, &ACharacterBase::EnableInputControl, StunDuration, false);
}

uint8 ACharacterBase::GetTeamID() const
{
	return TeamID;
}

void ACharacterBase::EnableInputControl()
{
	if (bIsDead)
	{
		return;
	}

	bIsStun = false;

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->EnableInput(PlayerController);
	}

	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->GetBrainComponent()->RestartLogic();
	}
}

void ACharacterBase::DisableInputControl()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->DisableInput(PlayerController);
	}

	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->GetBrainComponent()->StopLogic("DisableInput");
	}
}

void ACharacterBase::AddAbilityToUI(TSubclassOf<UGameplayAbilityBase> AbilityToAdd)
{
	APlayerControllerBase* PlayerControllerBase = Cast<APlayerControllerBase>(GetController());
	if (PlayerControllerBase)
	{
		UGameplayAbilityBase* AbilityInstance = AbilityToAdd.Get()->GetDefaultObject<UGameplayAbilityBase>();
		if (AbilityInstance)
		{
			FGameplayAbilityInfo AbilityInfo = AbilityInstance->GetAbilityInfo();
			PlayerControllerBase->AddAbilityToUI(AbilityInfo);
		}
	}
}