// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GrassComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE4GASP_API UGrassComponent : public UHierarchicalInstancedStaticMeshComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "SpawningGrass")
	FBox SpawningExtents;

	UPROPERTY(EditDefaultsOnly, Category = "SpawningGrass")
	int32 SpawnCount;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void SpawnGrass();	
};
