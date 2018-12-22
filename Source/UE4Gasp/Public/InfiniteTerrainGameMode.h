// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorPoolComponent.h"
#include "GameFramework/GameModeBase.h"
#include "InfiniteTerrainGameMode.generated.h"

UCLASS()
class UE4GASP_API AInfiniteTerrainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AInfiniteTerrainGameMode();

	UFUNCTION(BlueprintCallable, Category = "Bounds Pool")
	void PopulateBoundsVolumePool();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bounds Pool")
	UActorPoolComponent* NavMeshBoundsVolumePool;

private:
	void AddToPool(class ANavMeshBoundsVolume* VolumeToAdd);
};
