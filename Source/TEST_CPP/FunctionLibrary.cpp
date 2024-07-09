#include "FunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "TEST_CPPSaveGame.h"

bool UFunctionLibrary::SaveGame(UWorld* World, FString SlotName, int32 UserIndex)
{
	USaveGame* NewSaveGame = UGameplayStatics::CreateSaveGameObject(UOurSaveGame::StaticClass());
	if (NewSaveGame)
	{
		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(World, 0);
		
		UOurSaveGame* SaveGame = Cast<UOurSaveGame>(NewSaveGame);
		if (SaveGame)
		{
			SaveGame->Player.Location = PlayerCharacter->GetActorLocation();
			SaveGame->Player.Scale = PlayerCharacter->GetActorScale();
			SaveGame->Player.Rotation = PlayerCharacter->GetActorRotation();

			UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, UserIndex);
			return true;
		}
	}

	return false;
}

bool UFunctionLibrary::LoadGame(UWorld* World, FString SlotName, int32 UserIndex)
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(World, 0);
	UOurSaveGame* LoadGame = Cast<UOurSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));

	if (LoadGame)
	{
		PlayerCharacter->SetActorLocation(LoadGame->Player.Location);
		PlayerCharacter->SetActorScale3D(LoadGame->Player.Scale);
		PlayerCharacter->SetActorRotation(LoadGame->Player.Rotation);

		return true;
	}

	return false;
}

void UFunctionLibrary::BindDynamicMaterialInstance(UStaticMeshComponent* Mesh, UMaterialInstanceDynamic* MaterialInstance, FName ParameterName, FColor Color)
{
	if (MaterialInstance != nullptr)
	{
		if (Mesh->GetMaterial(0))
		{
			Mesh->SetMaterial(0, MaterialInstance);
			MaterialInstance->SetVectorParameterValue(ParameterName, Color);
		}	
	}
}

void UFunctionLibrary::SetDynamicMaterialInstanceParameter(UMaterialInstanceDynamic* MaterialInstance, FName ParameterName, FColor Color)
{
	if (MaterialInstance != nullptr)
	{
		MaterialInstance->SetVectorParameterValue(ParameterName, Color);
	}
}
