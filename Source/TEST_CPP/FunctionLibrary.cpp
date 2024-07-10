#include "FunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "MovableActor.h"
#include "TEST_CPPSaveGame.h"

bool UFunctionLibrary::SaveGame(UWorld* World, FString SlotName, int32 UserIndex)
{
	USaveGame* NewSaveGame = UGameplayStatics::CreateSaveGameObject(UOurSaveGame::StaticClass());
	if (NewSaveGame)
	{
		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(World, 0);
		
		TArray<AActor*> PushableActors;
		UGameplayStatics::GetAllActorsOfClassWithTag(World, AMovableActor::StaticClass(), FName("MovableByPushing"), PushableActors);

		TArray<AActor*> TelekinesisActors;
		UGameplayStatics::GetAllActorsOfClassWithTag(World, AMovableActor::StaticClass(), FName("MovableByTelekinesis"), TelekinesisActors);


		UOurSaveGame* SaveGame = Cast<UOurSaveGame>(NewSaveGame);
		if (SaveGame)
		{
			// Player
			SaveGame->Player.Location = PlayerCharacter->GetActorLocation();
			SaveGame->Player.Scale = PlayerCharacter->GetActorScale();
			SaveGame->Player.Rotation = PlayerCharacter->GetActorRotation();

			// Pushable
			for (AActor* Actor : PushableActors)
			{
				if (AMovableActor* MovableActor = Cast<AMovableActor>(Actor))
				{
					FMovableActorSave Save;
					Save.Location = MovableActor->GetActorLocation();
					Save.Scale = MovableActor->GetActorScale();
					Save.Rotation = MovableActor->GetActorRotation();
					Save.InteractionMode = MovableActor->GetInteractionMode();

					SaveGame->PushableActors.Add(Save);
				}
			}

			// Telekinesis
			for (AActor* Actor : TelekinesisActors)
			{
				if (AMovableActor* MovableActor = Cast<AMovableActor>(Actor))
				{
					FMovableActorSave Save;
					Save.Location = MovableActor->GetActorLocation();
					Save.Scale = MovableActor->GetActorScale();
					Save.Rotation = MovableActor->GetActorRotation();
					Save.InteractionMode = MovableActor->GetInteractionMode();

					SaveGame->TelekinesisActors.Add(Save);
				}
			}

			UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, UserIndex);
			UE_LOG(LogTemp, Warning, TEXT("Save written!"))
			return true;
		}
	}

	return false;
}

bool UFunctionLibrary::LoadGame(UWorld* World, FString SlotName, int32 UserIndex)
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(World, 0);

	TArray<AActor*> PushableActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(World, AMovableActor::StaticClass(), FName("MovableByPushing"), PushableActors);

	TArray<AActor*> TelekinesisActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(World, AMovableActor::StaticClass(), FName("MovableByTelekinesis"), TelekinesisActors);

	UOurSaveGame* LoadGame = Cast<UOurSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));

	if (LoadGame)
	{
		// Player
		PlayerCharacter->SetActorLocation(LoadGame->Player.Location);
		PlayerCharacter->SetActorScale3D(LoadGame->Player.Scale);
		PlayerCharacter->SetActorRotation(LoadGame->Player.Rotation);

		// Pushable
		if (PushableActors.Num() == LoadGame->PushableActors.Num())
		{
			for (size_t i = 0; i < PushableActors.Num(); i += 1)
			{
				AMovableActor* MovableActor = Cast<AMovableActor>(PushableActors[i]);
				if (MovableActor)
				{
					FMovableActorSave Save = LoadGame->PushableActors[i];

					MovableActor->SetActorLocation(Save.Location);
					MovableActor->SetActorScale3D(Save.Scale);
					MovableActor->SetActorRotation(Save.Rotation);
					MovableActor->SetInteractionMode(Save.InteractionMode);

					if (Save.InteractionMode == EInteractionMode::None)
					{
						MovableActor->SetColorByInteraction(EInteractionMode::Push);
					}
				}
			}
		}

		// Telekinesis
		if (TelekinesisActors.Num() == LoadGame->TelekinesisActors.Num())
		{
			for (size_t i = 0; i < TelekinesisActors.Num(); i += 1)
			{
				AMovableActor* MovableActor = Cast<AMovableActor>(TelekinesisActors[i]);
				if (MovableActor)
				{
					FMovableActorSave Save = LoadGame->TelekinesisActors[i];

					MovableActor->SetActorLocation(Save.Location);
					MovableActor->SetActorScale3D(Save.Scale);
					MovableActor->SetActorRotation(Save.Rotation);
					MovableActor->SetInteractionMode(Save.InteractionMode);

					if (Save.InteractionMode == EInteractionMode::None)
					{
						MovableActor->SetColorByInteraction(EInteractionMode::Telekinesis);
					}
				}
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("Save loaded!"))
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
