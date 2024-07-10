// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveTrigger.generated.h"

UCLASS()
class TEST_CPP_API ASaveTrigger : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Save", meta = (AllowPrivateAccess = "true"))
	int32 MaxNumOfSaves;

private:
	int32 TimesTriggered;
	FTimerHandle TimerHandle;

public:	
	ASaveTrigger();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	void TriggerGameSave();
};
