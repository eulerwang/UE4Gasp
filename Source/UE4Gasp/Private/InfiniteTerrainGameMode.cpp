// Fill out your copyright notice in the Description page of Project Settings.

#include "InfiniteTerrainGameMode.h"
#include "Navmesh/NavMeshBoundsVolume.h"
#include "EngineUtils.h"


// Sets default values
AInfiniteTerrainGameMode::AInfiniteTerrainGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	NavMeshBoundsVolumePool = CreateDefaultSubobject<UActorPoolComponent>(FName("NavMeshBoundsVolumePool"));
}


void AInfiniteTerrainGameMode::PopulateBoundsVolumePool()
{
	TActorIterator<ANavMeshBoundsVolume> VolumeIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());
	while (VolumeIterator)
	{
		AddToPool(*VolumeIterator);
		++VolumeIterator;
	}
}

void AInfiniteTerrainGameMode::AddToPool(ANavMeshBoundsVolume* VolumeToAdd)
{
	UE_LOG(LogTemp, Warning, TEXT("Found Volume: %s"), *VolumeToAdd->GetName());
	NavMeshBoundsVolumePool->Add(VolumeToAdd);
}
