// Copyright Epic Games, Inc. All Rights Reserved.

#include "TEST_CPPCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FunctionLibrary.h"
#include "MovableActor.h"
#include "MovableActor.h"
#include "Components/BoxComponent.h"


//////////////////////////////////////////////////////////////////////////
// ATEST_CPPCharacter

ATEST_CPPCharacter::ATEST_CPPCharacter()
	: InteractionDistance{ 1500.0f }, ObjectMovementStep{50.0f}
{
	PrimaryActorTick.bCanEverTick = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void ATEST_CPPCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATEST_CPPCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATEST_CPPCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATEST_CPPCharacter::Look);

		// Restart
		EnhancedInputComponent->BindAction(RestartAction, ETriggerEvent::Completed, this, &ATEST_CPPCharacter::Restart);

		// Interaction
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Completed, this, &ATEST_CPPCharacter::ToggleInteraction);

		// Object movement
		EnhancedInputComponent->BindAction(MoveObjectCloserOrFurtherAction, ETriggerEvent::Triggered, this, &ATEST_CPPCharacter::MoveObjectCloserOrFurther);
	}
}

void ATEST_CPPCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATEST_CPPCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);

		if (bIsInteracting)
		{
			float DistanceBetweenPlayerAndActor = (GetActorLocation() - ActorBeingInteractedWith->GetActorLocation()).Length();
			FVector UpdatedLocation = GetActorLocation() + (FollowCamera->GetForwardVector() * DistanceBetweenPlayerAndActor);
			ActorBeingInteractedWith->SetActorLocation(UpdatedLocation);
		}
	}
}

void ATEST_CPPCharacter::Restart(const FInputActionValue& Value)
{
	bool Result = UFunctionLibrary::LoadGame(GetWorld(), "MAIN", 0);
	if (Result)
	{
		UE_LOG(LogTemp, Warning, TEXT("Loading save file"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No save to load!"))
	}
}

void ATEST_CPPCharacter::ToggleInteraction(const FInputActionValue& Value)
{
	if (bIsInteracting)
	{
		ActorBeingInteractedWith->EnablePhysics();
		ActorBeingInteractedWith = nullptr;
		bIsInteracting = false;
		return;
	}

	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = Start + FollowCamera->GetForwardVector() * InteractionDistance;

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

	if (AActor* HitActor = HitResult.GetActor())
	{
		if (HitActor->ActorHasTag("Movable"))
		{
			ActorBeingInteractedWith = Cast<AMovableActor>(HitActor);
			if (ActorBeingInteractedWith)
			{
				UE_LOG(LogTemp, Warning, TEXT("Interacting with %s"), *HitActor->GetName())
				ActorBeingInteractedWith->DisablePhysics();
				ActorBeingInteractedWith->SetActorRotation(GetActorRotation());
				bIsInteracting = true;
			}
		}
	}
}

void ATEST_CPPCharacter::MoveObjectCloserOrFurther(const FInputActionValue& Value)
{
	if (!bIsInteracting) return;
	
	float ScrollValue = Value.Get<float>();
	FVector DirectionFromActorToPlayer = GetActorLocation() - ActorBeingInteractedWith->GetActorLocation();
	DirectionFromActorToPlayer.Z = 0.0f;
	DirectionFromActorToPlayer.Normalize();

	FVector UpdatedLocation = ActorBeingInteractedWith->GetActorLocation() + (DirectionFromActorToPlayer * ObjectMovementStep * ScrollValue);
	ActorBeingInteractedWith->SetActorLocation(UpdatedLocation);
}
