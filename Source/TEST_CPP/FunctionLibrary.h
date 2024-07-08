#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FunctionLibrary.generated.h"

UCLASS()
class TEST_CPP_API UFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static bool SaveGame(UWorld* World, FString SlotName, int32 UserIndex);

	UFUNCTION(BlueprintCallable)
	static bool LoadGame(UWorld* World, FString SlotName, int32 UserIndex);
};
