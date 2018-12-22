// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorPoolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4GASP_API UActorPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorPoolComponent();

	AActor* Checkout();
	void Return(AActor* ActorToReturn);
	void Add(AActor* ActorToAdd);

private:
	TArray<AActor*> Pool;
};
