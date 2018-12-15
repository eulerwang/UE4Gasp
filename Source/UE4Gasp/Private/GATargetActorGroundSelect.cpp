// Fill out your copyright notice in the Description page of Project Settings.

#include "GATargetActorGroundSelect.h"
#include "GameplayAbility.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Components/DecalComponent.h"
#include "Components/SceneComponent.h"

AGATargetActorGroundSelect::AGATargetActorGroundSelect()
{
	PrimaryActorTick.bCanEverTick = true;
	Radius = 200.0f;

	Decal = CreateDefaultSubobject<UDecalComponent>("Decal");
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(RootComponent);
	Decal->SetupAttachment(RootComponent);
	Decal->DecalSize = FVector(Radius);
}

void AGATargetActorGroundSelect::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	FVector LookingPoint = GetPlayerLookingPoint();
	Decal->SetWorldLocation(LookingPoint);
}

void AGATargetActorGroundSelect::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;
	MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
	Decal->DecalSize = FVector(Radius);
}

void AGATargetActorGroundSelect::ConfirmTargetingAndContinue()
{
	FVector ViewLocation = GetPlayerLookingPoint();

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

	FGameplayAbilityTargetData_LocationInfo* CenterLocation = new FGameplayAbilityTargetData_LocationInfo();
	if (Decal)
	{
		CenterLocation->TargetLocation.LiteralTransform = Decal->GetComponentTransform();
		CenterLocation->TargetLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;

	}

	if (OverlapedActors.Num() > 0)
	{
		FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromActors(OverlapedActors);
		TargetData.Add(CenterLocation);
		TargetDataReadyDelegate.Broadcast(TargetData);
	}
	else
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle(CenterLocation));
	}
}

FVector AGATargetActorGroundSelect::GetPlayerLookingPoint()
{
	FVector ViewPoint;
	FRotator ViewRotation;
	MasterPC->GetPlayerViewPoint(ViewPoint, ViewRotation);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;

	APawn* MasterPawn = MasterPC->GetPawn();
	if (MasterPawn)
	{
		QueryParams.AddIgnoredActor(MasterPawn);
	}

	FVector LookAtPoint = FVector();
	bool TryTrace = GetWorld()->LineTraceSingleByChannel(HitResult, ViewPoint, ViewPoint + ViewRotation.Vector() * 10000.0f, ECC_Visibility, QueryParams);
	if (TryTrace)
	{
		LookAtPoint = HitResult.ImpactPoint;
	}
	
	return LookAtPoint;
}
