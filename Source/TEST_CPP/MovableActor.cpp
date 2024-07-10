// Fill out your copyright notice in the Description page of Project Settings.

#include "MovableActor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FunctionLibrary.h"

AMovableActor::AMovableActor()
	: IdleColor{FColor::White}, InteractionColor{FColor::Cyan}, TelekinesisColor{FColor::Purple}, PushColor{FColor::Orange}
	,InteractionMode{EInteractionMode::None}
{
	PrimaryActorTick.bCanEverTick = false;

	// Collider
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	
	SetRootComponent(BoxComponent);

	// Mesh
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(BoxComponent);
}

void AMovableActor::BeginPlay()
{
	Super::BeginPlay();

	UMaterial* BaseMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/ThirdPerson/Materials/M_SolidParam.M_SolidParam"));
	MaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, MeshComponent);

	if (ActorHasTag("MovableByTelekinesis"))
		UFunctionLibrary::BindDynamicMaterialInstance(MeshComponent, MaterialInstance, "BaseColor", TelekinesisColor);
	else if (ActorHasTag("MovableByPushing"))
		UFunctionLibrary::BindDynamicMaterialInstance(MeshComponent, MaterialInstance, "BaseColor", PushColor);
	else
		UFunctionLibrary::BindDynamicMaterialInstance(MeshComponent, MaterialInstance, "BaseColor", IdleColor);
	
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

void AMovableActor::BeginInteraction(const EInteractionMode Mode)
{
	const ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (Player == nullptr) return;

	switch (Mode)
	{
		case EInteractionMode::Telekinesis:
		{
			DisablePhysics();
			SetActorRotation(Player->GetActorRotation());
			Player->GetCharacterMovement()->SetJumpAllowed(false);
			UFunctionLibrary::SetDynamicMaterialInstanceParameter(MaterialInstance, "BaseColor", InteractionColor);
			InteractionMode = Mode;
		} break;

		case EInteractionMode::Push:
		{
			DisablePhysics();
			Player->GetCharacterMovement()->bOrientRotationToMovement = false;
			Player->GetCharacterMovement()->SetJumpAllowed(false);
			UFunctionLibrary::SetDynamicMaterialInstanceParameter(MaterialInstance, "BaseColor", InteractionColor);
			InteractionMode = Mode;
		} break;

		default:
		{
			InteractionMode = EInteractionMode::None;
			UFunctionLibrary::SetDynamicMaterialInstanceParameter(MaterialInstance, "BaseColor", IdleColor);
		} break;
	}
}

void AMovableActor::EndInteraction()
{
	const ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (Player == nullptr) return;

	switch (InteractionMode)
	{
		case EInteractionMode::Telekinesis:
		{
			EnablePhysics();
			Player->GetCharacterMovement()->SetJumpAllowed(true);
			SetColorByInteraction(InteractionMode);
			InteractionMode = EInteractionMode::None;
		} break;

		case EInteractionMode::Push:
		{
			EnablePhysics();
			Player->GetCharacterMovement()->bOrientRotationToMovement = true;
			Player->GetCharacterMovement()->SetJumpAllowed(true);
			SetColorByInteraction(InteractionMode);
			InteractionMode = EInteractionMode::None;
		} break;

		default:
		{
			SetColorByInteraction(InteractionMode);
		} break;
	}
}

void AMovableActor::SetColorByInteraction(const EInteractionMode Interaction)
{
	switch (Interaction)
	{
		case EInteractionMode::Telekinesis:
		{
			UFunctionLibrary::SetDynamicMaterialInstanceParameter(MaterialInstance, "BaseColor", TelekinesisColor);
		} break;

		case EInteractionMode::Push:
		{
			UFunctionLibrary::SetDynamicMaterialInstanceParameter(MaterialInstance, "BaseColor", PushColor);
		} break;

		default:
		{
			UFunctionLibrary::SetDynamicMaterialInstanceParameter(MaterialInstance, "BaseColor", IdleColor);
		} break;
	}
}
