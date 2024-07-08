// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovableActor.generated.h"

UCLASS()
class TEST_CPP_API AMovableActor : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComponent;

public:	
	AMovableActor();
	
protected:
	virtual void BeginPlay() override;

public:
	void EnablePhysics() const;
	void DisablePhysics() const;

	FORCEINLINE UBoxComponent* GetBoxComponent() const { return BoxComponent; }
	FORCEINLINE UStaticMeshComponent* GetMeshComponent() const { return MeshComponent; }
};
