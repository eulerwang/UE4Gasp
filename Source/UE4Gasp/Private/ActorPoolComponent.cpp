// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPoolComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UActorPoolComponent::UActorPoolComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

AActor* UActorPoolComponent::Checkout()
{
	if (Pool.Num() == 0)
	{
		return nullptr;
	}

	return Pool.Pop();
}

void UActorPoolComponent::Return(AActor* ActorToReturn)
{
	Add(ActorToReturn);
}

void UActorPoolComponent::Add(AActor* ActorToAdd)
{
	Pool.Push(ActorToAdd);
}
