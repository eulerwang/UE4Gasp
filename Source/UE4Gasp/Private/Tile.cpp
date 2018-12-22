// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "NavigationSystem.h"
#include "Engine/World.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	MinExtent = FVector(0, -2000, 0);
	MaxExtent = FVector(8000, 2000, 0);
	NavigationBoundsOffset = FVector(4000, 0, 0);
}

void ATile::SetActorPoolComponent(UActorPoolComponent* InActorPoolComponent)
{
	ActorPoolComponent = InActorPoolComponent;
	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = ActorPoolComponent->Checkout();
	if (!NavMeshBoundsVolume)
	{
		return;
	}

	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
	
	// UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	// NavSystem->SetGeometryGatheringMode(ENavDataGatheringModeConfig::Lazy);
	// NavSystem->Build();
	FNavigationSystem::Build(*GetWorld());
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (ActorPoolComponent && NavMeshBoundsVolume)
	{
		ActorPoolComponent->Return(NavMeshBoundsVolume);
	}
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float MinScale, float MaxScale, float Radius)
{
	FBox Bounds = FBox(MinExtent, MaxExtent);
	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);

	// Calculate Spawn Positions and Place Actors
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
		bool CanSpawn = FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale, Bounds);
		if (CanSpawn)
		{
			PlaceActor(ToSpawn, SpawnPosition);
		}
	}
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float Radius)
{
	FBox Bounds = FBox(MinExtent, MaxExtent);
	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);

	// Calculate Spawn Positions and Place AI Pawns
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FSpawnPosition SpawnPosition;
		bool CanSpawn = FindEmptyLocation(SpawnPosition.Location, Radius, Bounds);
		if (CanSpawn)
		{
			PlaceAIPawn(ToSpawn, SpawnPosition);
		}
	}
}

bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius, FBox Bounds)
{
	for (size_t i = 0; i < MAX_ALLOWED_CAST_SPHERE; i++)
	{
		FVector RandomPoint = FMath::RandPointInBox(Bounds);
		if (CanSpawnAtLocation(RandomPoint, Radius)) {
			OutLocation = RandomPoint;
			return true;
		}
	}

	return false;
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition)
{
	AActor* ActorSpawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	ActorSpawned->SetActorRelativeLocation(SpawnPosition.Location);
	
	FRotator RandomRotation = FRotator(0, FMath::RandRange(-180.0f, 180.0f), 0);
	ActorSpawned->SetActorRotation(RandomRotation, ETeleportType::None);

	ActorSpawned->SetActorScale3D(FVector(SpawnPosition.Scale));

	ActorSpawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
}

void ATile::PlaceAIPawn(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition)
{
	FRotator Rotation = FRotator(0, FMath::RandRange(-180.0f, 180.0f), 0);
	APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(ToSpawn, SpawnPosition.Location, Rotation);
	if (!SpawnedPawn)
	{
		return;
	}

	SpawnedPawn->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	SpawnedPawn->SpawnDefaultController();
}

bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GlobalLocation,
		GlobalLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);

	return !HasHit;
}