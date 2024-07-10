// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveTrigger.h"
#include "Components/BoxComponent.h"
#include "FunctionLibrary.h"

ASaveTrigger::ASaveTrigger()
	: MaxNumOfSaves{ 2 }
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
	if (TimesTriggered >= MaxNumOfSaves) return;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASaveTrigger::TriggerGameSave, 3.0f, false);
}

void ASaveTrigger::TriggerGameSave()
{
	UE_LOG(LogTemp, Warning, TEXT("Manual save.."))
	UFunctionLibrary::SaveGame(GetWorld(), "MAIN", 0);

	GetWorldTimerManager().ClearTimer(TimerHandle);
	TimesTriggered += 1;
}
