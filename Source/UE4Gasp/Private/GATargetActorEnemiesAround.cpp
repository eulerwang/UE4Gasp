// Fill out your copyright notice in the Description page of Project Settings.

#include "GATargetActorEnemiesAround.h"
#include "GameplayAbility.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

void AGATargetActorEnemiesAround::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;
	MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
}

void AGATargetActorEnemiesAround::ConfirmTargetingAndContinue()
{
	APawn* OwningPawn = MasterPC->GetPawn();
	if (!OwningPawn)
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
		return;
	}

	FVector ViewLocation = OwningPawn->GetActorLocation();

	TArray<FOverlapResult> Overlaps;
	TArray<TWeakObjectPtr<AActor>> OverlapedActors;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bTraceComplex = false;
	CollisionQueryParams.bReturnPhysicalMaterial = false;
	CollisionQueryParams.AddIgnoredActor(MasterPC->GetPawn());

	bool TryOverlap = GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		ViewLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECC_Pawn),
		FCollisionShape::MakeSphere(Radius),
		CollisionQueryParams
	);

	if (TryOverlap)
	{
		for (FOverlapResult OverlapResult : Overlaps)
		{
			APawn* PawnOverlaped = Cast<APawn>(OverlapResult.GetActor());
			if (PawnOverlaped && !OverlapedActors.Contains(PawnOverlaped))
			{
				OverlapedActors.Add(PawnOverlaped);
			}
		}
	}

	if (OverlapedActors.Num() > 0)
	{
		FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromActors(OverlapedActors);
		TargetDataReadyDelegate.Broadcast(TargetData);
	} else
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
	}
}
