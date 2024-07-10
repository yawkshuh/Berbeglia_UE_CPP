// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TEST_CPPGameMode.generated.h"

UCLASS(minimalapi)
class ATEST_CPPGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	FTimerHandle TimerHandle;

private:
	virtual void BeginPlay() override;
	void TriggerSave() const;

public:
	ATEST_CPPGameMode();
};



