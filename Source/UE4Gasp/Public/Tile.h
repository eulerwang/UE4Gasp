// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorPoolComponent.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

struct FSpawnPosition
{
	FVector Location;
	float Scale;
};

UCLASS()
class UE4GASP_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION(BlueprintCallable, Category = "Tile")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float MinScale = 1.0f, float MaxScale = 1.0f, float Radius = 500.0f);

	UFUNCTION(BlueprintCallable, Category = "Tile")
	void PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float Radius = 500.0f);

	UFUNCTION(BlueprintCallable, Category = "ActorPoolComponent")
	void SetActorPoolComponent(UActorPoolComponent* InActorPoolComponent);

	UPROPERTY(EditDefaultsOnly, Category = "Tile")
	FVector MinExtent;

	UPROPERTY(EditDefaultsOnly, Category = "Tile")
	FVector MaxExtent;

	UPROPERTY(EditDefaultsOnly, Category = "AI Navigation")
	FVector NavigationBoundsOffset;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UActorPoolComponent* ActorPoolComponent;
	AActor* NavMeshBoundsVolume;

	const int32 MAX_ALLOWED_CAST_SPHERE = 10;

	void PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition);
	void PlaceAIPawn(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition);
	bool FindEmptyLocation(FVector& OutLocation, float Radius, FBox Bounds);
	bool CanSpawnAtLocation(FVector Location, float Radius);
	void PositionNavMeshBoundsVolume();
};
