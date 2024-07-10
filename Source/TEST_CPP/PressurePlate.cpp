// Fill out your copyright notice in the Description page of Project Settings.

#include "PressurePlate.h"
#include "FunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TEST_CPPGameStateBase.h"


APressurePlate::APressurePlate()
	: IdleColor{FColor::Red}, ActiveColor{FColor::Green}, MaterialInstance{ nullptr }
{
	PrimaryActorTick.bCanEverTick = false;

	// Collider
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APressurePlate::OnBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &APressurePlate::OnEndOverlap);

	SetRootComponent(BoxComponent);

	// Mesh
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(BoxComponent);
}

void APressurePlate::BeginPlay()
{
	Super::BeginPlay();
	
	UMaterial* BaseMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/ThirdPerson/Materials/M_SolidParam.M_SolidParam"));
	MaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, MeshComponent);
	UFunctionLibrary::BindDynamicMaterialInstance(MeshComponent, MaterialInstance, "BaseColor", IdleColor);
}

void APressurePlate::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	UFunctionLibrary::SetDynamicMaterialInstanceParameter(MaterialInstance, "BaseColor", ActiveColor);
	
	ATEST_CPPGameStateBase* GameState = Cast<ATEST_CPPGameStateBase>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameState->SetPlatePressed(true);
	}
}

void APressurePlate::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	UFunctionLibrary::SetDynamicMaterialInstanceParameter(MaterialInstance, "BaseColor", IdleColor);
	ATEST_CPPGameStateBase* GameState = Cast<ATEST_CPPGameStateBase>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameState->SetPlatePressed(false);
	}
}
