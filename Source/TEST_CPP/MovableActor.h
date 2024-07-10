// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionMode.h"
#include "MovableActor.generated.h"

UCLASS()
class TEST_CPP_API AMovableActor : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Material, meta = (AllowPrivateAccess = "true"))
	FColor IdleColor;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Material, meta = (AllowPrivateAccess = "true"))
	FColor InteractionColor;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Material, meta = (AllowPrivateAccess = "true"))
	FColor TelekinesisColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Material, meta = (AllowPrivateAccess = "true"))
	FColor PushColor;

	UPROPERTY()
	UMaterialInstanceDynamic* MaterialInstance;
	
private:
	EInteractionMode InteractionMode;
	
public:	
	AMovableActor();
	
protected:
	virtual void BeginPlay() override;

private:
	void EnablePhysics() const;
	void DisablePhysics() const;

public:
	void BeginInteraction(const EInteractionMode Mode);
	void EndInteraction();

	FORCEINLINE UBoxComponent* GetBoxComponent() const { return BoxComponent; }
	FORCEINLINE UStaticMeshComponent* GetMeshComponent() const { return MeshComponent; }
};
