#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FunctionLibrary.generated.h"

UCLASS()
class TEST_CPP_API UFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
#pragma region "Save Game"
	UFUNCTION(BlueprintCallable)
	static bool SaveGame(UWorld* World, FString SlotName, int32 UserIndex);

	UFUNCTION(BlueprintCallable)
	static bool LoadGame(UWorld* World, FString SlotName, int32 UserIndex);
#pragma endregion

#pragma region "Dynamic Materials"
	UFUNCTION(BlueprintCallable)
	static void BindDynamicMaterialInstance(UStaticMeshComponent* Mesh, UMaterialInstanceDynamic* MaterialInstance, FName ParameterName, FColor Color);

	UFUNCTION(BlueprintCallable)
	static void SetDynamicMaterialInstanceParameter(UMaterialInstanceDynamic* MaterialInstance, FName ParameterName, FColor Color);
#pragma endregion
};
