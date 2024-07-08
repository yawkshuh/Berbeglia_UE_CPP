// Copyright Epic Games, Inc. All Rights Reserved.

#include "TEST_CPPGameMode.h"
#include "TEST_CPPCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATEST_CPPGameMode::ATEST_CPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
