// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TEST_CPPGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class TEST_CPP_API ATEST_CPPGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsPlatePressed;

public:
	FORCEINLINE void SetPlatePressed(bool Pressed) { bIsPlatePressed = Pressed; }
	FORCEINLINE bool IsPlatePressed() const { return bIsPlatePressed; }
};
