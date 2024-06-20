#pragma once

#include "CoreMinimal.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GP3CameraShake.h"
#include "Components/PostProcessComponent.h"
#include "GP3CharacterMovement.generated.h"


UENUM(BlueprintType)
enum class ESizeState : uint8
{
	Small,
	Normal,
	Big
};

struct VectorArray
{
	VectorArray(FVector check, FVector jump) : CheckDirection(check), JumpDirection(jump) {}
	FVector CheckDirection;
	FVector JumpDirection;
};

UCLASS()
class AGP3CharacterMovement : public ACharacter
{
	GENERATED_BODY()

public:
	//Enhanced Input System mappings & actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* PauseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ZoomAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LedgeMantleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* DropAction;

	AGP3CharacterMovement();

	UPROPERTY(BlueprintReadWrite)
	float CamLengthNormal;

	UPROPERTY(BlueprintReadWrite)
	float CamLengthChanged;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	ESizeState SizeState;

	UPROPERTY(EditAnywhere)
	int ConsumedGBeans = 0;

	UPROPERTY(EditAnywhere)
	int ConsumedSBeans = 0;


	UPROPERTY(BlueprintReadOnly)
	bool isClimbing;

	UPROPERTY(BlueprintReadOnly)
	bool isClimbingUp;

	UPROPERTY(BlueprintReadOnly)
	bool bClimbJumping;

	UPROPERTY(BlueprintReadOnly)
	bool IsSliding;

	UFUNCTION(BlueprintCallable)
	void LedgeMantle();

private:

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void isJumping();
	void Zoom(const FInputActionValue& Value);
	void Sprint();
	void StopSprint();
	void CanRampSlide();
	void ResetSlide();
	void HandleClimbing();
	void ResetClimbingUp();

	UFUNCTION()
	void HandleDrop();

	UPROPERTY(VisibleAnywhere)
	bool Dropped = false;

	UPROPERTY(VisibleAnywhere)
	AActor* LatestHitActor = nullptr;

	UPROPERTY(VisibleAnywhere)
	bool IsGrounded = true;
	
	UPROPERTY(VisibleAnywhere)
	bool IsAirborne = false;

	UPROPERTY(EditAnywhere)
	float WallClimbJumpForce = 1400.f;

	UPROPERTY(EditAnywhere)
	float WallClimbJumpDistance = 300.f;

	UPROPERTY(EditAnywhere)
	UCameraComponent* camcorder;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* camArm;

	UPROPERTY(EditAnywhere, Category = "CharacterMovement", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float WalkingSpeedMultiplier;

	UPROPERTY(EditAnywhere, Category = "CharacterMovement", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float SprintSpeedMultiplier;

	UPROPERTY(EditAnywhere, Category = "CharacterMovement", meta = (ClampMin = "0", ClampMax = "90"))
	float SlopeAngleThreshold;

	UPROPERTY()
	FVector2D MoveVector;

	UPROPERTY(VisibleAnywhere)
	bool Jumping;

	bool IsOnSteepSlope;
	bool isWalking;

	float ForwardInput;
	float RightInput;

	UPROPERTY(EditAnywhere, Category = "Character Mesh")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	FVector LedgeLocation;

	void FindNewClimbingPoint();
	void UpdateClimbingRotation();
	void CheckForClimbableJump();
	void PerformClimbJump(FVector JumpDirection);
	FVector ClimbJumpTarget;
	FVector CurrentClimbableLocation;

	void HandleClimbingEdges();

	AActor* CurrentClimbableActor;
	UGP3CameraShake* SprintCameraShake;

	float NormalFOV;
	float SprintFOV;
	float FOVInterpSpeed;

	UPROPERTY(EditAnywhere)
	float NormalMotionBlurAmount;

	UPROPERTY(EditAnywhere)
	float SprintMotionBlurAmount;

	UPROPERTY(EditAnywhere)
	UPostProcessComponent* PostProcessComponent;
};