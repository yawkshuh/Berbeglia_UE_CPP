// Copyright Epic Games, Inc. All Rights Reserved.

#include "TEST_CPPGameMode.h"
#include "TEST_CPPCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "FunctionLibrary.h"

ATEST_CPPGameMode::ATEST_CPPGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ATEST_CPPGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ATEST_CPPGameMode::TriggerSave, 60.0f, true);
}

void ATEST_CPPGameMode::TriggerSave() const
{
	UE_LOG(LogTemp, Warning, TEXT("Auto-save triggered.."))
	UFunctionLibrary::SaveGame(GetWorld(), "MAIN", 0);
}
