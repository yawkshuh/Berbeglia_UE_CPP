// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "InteractionMode.h"
#include "TEST_CPPSaveGame.generated.h"

USTRUCT()
struct FPlayerSave
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FVector Location;

	UPROPERTY(VisibleAnywhere)
	FVector Scale;

	UPROPERTY(VisibleAnywhere)
	FRotator Rotation;
};

USTRUCT()
struct FMovableActorSave
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FVector Location;

	UPROPERTY(VisibleAnywhere)
	FVector Scale;

	UPROPERTY(VisibleAnywhere)
	FRotator Rotation;

	UPROPERTY(VisibleAnywhere)
	EInteractionMode InteractionMode;
};

UCLASS()
class TEST_CPP_API UOurSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = "Player")
	FPlayerSave Player;

	UPROPERTY(VisibleAnywhere, Category = "Movable Actors | Pushable")
	TArray<FMovableActorSave> PushableActors;

	UPROPERTY(VisibleAnywhere, Category = "Movable Actors | Telekinesis")
	TArray<FMovableActorSave> TelekinesisActors;
};
