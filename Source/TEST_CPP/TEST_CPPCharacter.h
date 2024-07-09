// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "InteractionMode.h"
#include "TEST_CPPCharacter.generated.h"

UCLASS(config=Game)
class ATEST_CPPCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	// Camera

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	// Input actions

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* RestartAction;

	// Interaction inputs
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Telekinesis", meta = (AllowPrivateAccess = "true"))
	class UInputAction* ToggleTelekinesisAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Telekinesis", meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveWithTelekinesisAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Pushing", meta = (AllowPrivateAccess = "true"))
	class UInputAction* TogglePushPullAction;

	// Telekinesis
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movable Objects", meta = (AllowPrivateAccess = "true"))
	float InteractionDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movable Objects", meta = (AllowPrivateAccess = "true"))
	float ObjectMovementStep;

	// Object pushing
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movable Objects", meta = (AllowPrivateAccess = "true"))
	float MaxPushDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movable Objects", meta = (AllowPrivateAccess = "true"))
	float ObjectOffsetWhileBeingPushed;

	// Actor we're currently interacting with, or nullptr if we're not interacting.
	UPROPERTY()
	class AMovableActor* InteractedActor;
	
private:
	// Interaction mode we're in, if any.
	EInteractionMode CurrentInteraction;

	// Direction we're pushing towards using the normal from the line trace hit.
	FVector PushDirection;

	// Distance from the object when we begin the interaction. 
	float InitialDistance;
	
public:
	ATEST_CPPCharacter();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	
private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void Restart(const FInputActionValue& Value);

	void ToggleTelekinesis(const FInputActionValue& Value);
	void TogglePushPull(const FInputActionValue& Value);
	void MoveObjectWithTelekinesis(const FInputActionValue& Value);

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

