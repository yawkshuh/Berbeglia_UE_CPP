// Fill out your copyright notice in the Description page of Project Settings.

#include "WinTrigger.h"
#include "Components/BoxComponent.h"
#include "TEST_CPPGameStateBase.h"

AWinTrigger::AWinTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWinTrigger::OnBeginOverlap);
}

void AWinTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	ATEST_CPPGameStateBase* GameState = Cast<ATEST_CPPGameStateBase>(GetWorld()->GetGameState());
	if (GameState)
	{
		if (GameState->IsPlatePressed())
		{
			UE_LOG(LogTemp, Warning, TEXT("YOU WIN!"))
		}
	}
}
