// Copyright Epic Games, Inc. All Rights Reserved.

#include "TEST_CPPCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FunctionLibrary.h"
#include "MovableActor.h"

ATEST_CPPCharacter::ATEST_CPPCharacter()
	: InteractionDistance{ 1500.0f }, ObjectMovementStep{50.0f}
	, MaxPushDistance{150.0f}, ObjectOffsetWhileBeingPushed{20.0f}
	, CurrentInteraction{EInteractionMode::None}
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
	CameraBoom->TargetOffset.Z = 130.0f;
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
		EnhancedInputComponent->BindAction(ToggleTelekinesisAction, ETriggerEvent::Completed, this, &ATEST_CPPCharacter::ToggleTelekinesis);
		EnhancedInputComponent->BindAction(TogglePushPullAction, ETriggerEvent::Completed, this, &ATEST_CPPCharacter::TogglePushPull);

		// Object movement
		EnhancedInputComponent->BindAction(MoveWithTelekinesisAction, ETriggerEvent::Triggered, this, &ATEST_CPPCharacter::MoveObjectWithTelekinesis);
	}
}

void ATEST_CPPCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if (CurrentInteraction == EInteractionMode::Push)
		{
			FVector DirectionFromPlayerToObject = PushDirection;
			DirectionFromPlayerToObject.Z = 0.0f;

			const FVector UpdatedLocation = GetActorLocation() + (DirectionFromPlayerToObject * (InitialDistance + ObjectOffsetWhileBeingPushed));

			AddMovementInput(PushDirection, MovementVector.Y);
			InteractedActor->SetActorLocation(UpdatedLocation);
			return;
		}
		
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATEST_CPPCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);

		if (CurrentInteraction == EInteractionMode::Telekinesis)
		{
			const float DistanceBetweenPlayerAndActor = (GetActorLocation() - InteractedActor->GetActorLocation()).Length();
			const FVector UpdatedLocation = GetActorLocation() + (FollowCamera->GetForwardVector() * DistanceBetweenPlayerAndActor);
			InteractedActor->SetActorLocation(UpdatedLocation);
		}
	}
}

void ATEST_CPPCharacter::Restart(const FInputActionValue& Value)
{
	const bool Result = UFunctionLibrary::LoadGame(GetWorld(), "MAIN", 0);
	if (Result)
	{
		UE_LOG(LogTemp, Warning, TEXT("Loading save file"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No save to load!"))
	}
}

void ATEST_CPPCharacter::ToggleTelekinesis(const FInputActionValue& Value)
{
	if (CurrentInteraction == EInteractionMode::Telekinesis)
	{
		InteractedActor->EndInteraction();
		InteractedActor = nullptr;
		
		CurrentInteraction = EInteractionMode::None;
		return;
	}

	if (CurrentInteraction != EInteractionMode::None) return;
	
	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = Start + FollowCamera->GetForwardVector() * InteractionDistance;

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

	AActor* HitActor = HitResult.GetActor();
	if (HitActor == nullptr) return;
	if (!HitActor->ActorHasTag("MovableByTelekinesis")) return;

	InteractedActor = Cast<AMovableActor>(HitActor);
	if (InteractedActor == nullptr) return;

	InteractedActor->BeginInteraction(EInteractionMode::Telekinesis);
	CurrentInteraction = EInteractionMode::Telekinesis;
}

void ATEST_CPPCharacter::TogglePushPull(const FInputActionValue& Value)
{
	if (CurrentInteraction == EInteractionMode::Push)
	{
		InteractedActor->EndInteraction();
		InteractedActor = nullptr;

		CurrentInteraction = EInteractionMode::None;
		return;
	}
	
	if (CurrentInteraction != EInteractionMode::None) return;
	
	const UArrowComponent* Arrow = GetArrowComponent();
	const FVector Start = Arrow->GetComponentLocation();
	const FVector End = Start + Arrow->GetForwardVector() * MaxPushDistance;

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

	AActor* HitActor = HitResult.GetActor();
	if (HitActor == nullptr) return;
	if (!HitActor->ActorHasTag("MovableByPushing")) return;

	InteractedActor = Cast<AMovableActor>(HitActor);
	if (InteractedActor == nullptr) return;

	CurrentInteraction = EInteractionMode::Push;
	PushDirection = -(HitResult.Normal);

	InitialDistance = (HitActor->GetActorLocation() - GetActorLocation()).Length();
	const FVector SnappedLocation = HitActor->GetActorLocation() + (HitResult.Normal * InitialDistance);
			
	SetActorLocation(SnappedLocation);
	SetActorRotation(PushDirection.Rotation());
			
	InteractedActor->BeginInteraction(EInteractionMode::Push);
}

void ATEST_CPPCharacter::MoveObjectWithTelekinesis(const FInputActionValue& Value)
{
	if (CurrentInteraction != EInteractionMode::Telekinesis) return;
	
	const float ScrollValue = Value.Get<float>();
	FVector DirectionFromActorToPlayer = GetActorLocation() - InteractedActor->GetActorLocation();
	DirectionFromActorToPlayer.Z = 0.0f;
	DirectionFromActorToPlayer.Normalize();

	const FVector UpdatedLocation = InteractedActor->GetActorLocation() + (DirectionFromActorToPlayer * ObjectMovementStep * ScrollValue);
	InteractedActor->SetActorLocation(UpdatedLocation);
}
