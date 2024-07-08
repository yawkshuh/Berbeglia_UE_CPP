// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveTrigger.h"
#include "Components/BoxComponent.h"
#include "FunctionLibrary.h"

ASaveTrigger::ASaveTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	// Collider
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASaveTrigger::OnBeginOverlap);

	SetRootComponent(BoxComponent);
}

void ASaveTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void ASaveTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (bHasBeenUsed) return;
	//TriggerGameSave();

	if (!TimerHandle.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Save Triggered..."))
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ASaveTrigger::TriggerGameSave, 3.0f, false);
	}
}

void ASaveTrigger::TriggerGameSave()
{
	UE_LOG(LogTemp, Warning, TEXT("SAVING..."))
	UFunctionLibrary::SaveGame(GetWorld(), "MAIN", 0);
	bHasBeenUsed = true;
	GetWorldTimerManager().ClearTimer(TimerHandle);
}
