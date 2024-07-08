// Fill out your copyright notice in the Description page of Project Settings.

#include "MovableActor.h"
#include "Components/BoxComponent.h"

AMovableActor::AMovableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Collider
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	
	SetRootComponent(BoxComponent);

	// Mesh
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(BoxComponent);
	
	Tags.Add("Movable");
}

void AMovableActor::BeginPlay()
{
	Super::BeginPlay();
	EnablePhysics();	
}

void AMovableActor::EnablePhysics() const
{
	BoxComponent->SetSimulatePhysics(true);
	BoxComponent->SetMassOverrideInKg(NAME_None, 100000.0f, true);
}

void AMovableActor::DisablePhysics() const
{
	BoxComponent->SetSimulatePhysics(false);
}
