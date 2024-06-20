#include "GP3CharacterMovement.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include <Kismet/KismetMathLibrary.h>
#include "DrawDebugHelpers.h"

AGP3CharacterMovement::AGP3CharacterMovement()
{
	//Setting up default values
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	camcorder = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	camArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	camArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	camArm->TargetArmLength = 300.f;
	camArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));
	camArm->bUsePawnControlRotation = true;

	camcorder->AttachToComponent(camArm, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);

	Jumping = false;
	IsOnSteepSlope = false;
	IsSliding = false;
	isWalking = true;

	WalkingSpeedMultiplier = 0.5f;
	SprintSpeedMultiplier = 0.8f;
	SlopeAngleThreshold = 20.0f;

	SizeState = ESizeState::Normal;

	bClimbJumping = false;
	SprintCameraShake = nullptr;

	NormalFOV = 90.0f;
	SprintFOV = 110.0f;
	FOVInterpSpeed = 10.0f;

	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	PostProcessComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	NormalMotionBlurAmount = 0.0f;
	SprintMotionBlurAmount = 1.5f;

}

void AGP3CharacterMovement::BeginPlay()
{
	Super::BeginPlay();

	//Enable input mapping
	if(APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if(UEnhancedInputLocalPlayerSubsystem* EIS = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			EIS->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AGP3CharacterMovement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(GetCharacterMovement()->IsMovingOnGround())
	{
		Dropped = false;
	}

	if (!isClimbingUp && !bClimbJumping)
	{
		HandleClimbing();
	}

	if (isClimbing)
	{
		HandleClimbingEdges();
		UpdateClimbingRotation();
	}

	if (Jumping)
	{
		if (isClimbing)
		{
			CheckForClimbableJump();
		}
	}
	CanRampSlide();

	FVector Velocity = GetCharacterMovement()->Velocity;
	if (!isWalking && Velocity.Size() > 0)
	{
		if (!SprintCameraShake)
		{
			SprintCameraShake = Cast<UGP3CameraShake>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(UGP3CameraShake::StaticClass()));
		}
		camcorder->FieldOfView = FMath::FInterpTo(camcorder->FieldOfView, SprintFOV, DeltaTime, FOVInterpSpeed);
		PostProcessComponent->Settings.MotionBlurAmount = FMath::FInterpTo(PostProcessComponent->Settings.MotionBlurAmount, SprintMotionBlurAmount, DeltaTime, FOVInterpSpeed);
	}
	else
	{
		if (SprintCameraShake)
		{
			UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StopCameraShake(Cast<UCameraShakeBase>(SprintCameraShake), true);
			SprintCameraShake = nullptr;
		}
		camcorder->FieldOfView = FMath::FInterpTo(camcorder->FieldOfView, NormalFOV, DeltaTime, FOVInterpSpeed);
		PostProcessComponent->Settings.MotionBlurAmount = FMath::FInterpTo(PostProcessComponent->Settings.MotionBlurAmount, NormalMotionBlurAmount, DeltaTime, FOVInterpSpeed);
	}

}



void AGP3CharacterMovement::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGP3CharacterMovement::Move);	
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGP3CharacterMovement::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AGP3CharacterMovement::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AGP3CharacterMovement::isJumping);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AGP3CharacterMovement::Zoom);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AGP3CharacterMovement::StopSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AGP3CharacterMovement::Sprint);
		EnhancedInputComponent->BindAction(LedgeMantleAction, ETriggerEvent::Started, this, &AGP3CharacterMovement::LedgeMantle);
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Started, this, &AGP3CharacterMovement::HandleDrop);
	}
}

void AGP3CharacterMovement::Move(const FInputActionValue& Value)
{
	MoveVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		if (!isClimbing)
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			if(isWalking)
			{
				MoveVector.Normalize();
				AddMovementInput(ForwardVector, MoveVector.Y * WalkingSpeedMultiplier);
				AddMovementInput(RightVector, MoveVector.X * WalkingSpeedMultiplier);
			}
			else
			{
				MoveVector.Normalize();
				AddMovementInput(ForwardVector, MoveVector.Y * SprintSpeedMultiplier);
				AddMovementInput(RightVector, MoveVector.X * SprintSpeedMultiplier);
			}
		}
		else
		{
			HandleClimbingEdges();
			//Right movement math
			FVector CameraRight = camcorder->GetRightVector();
			FVector ClimbingPlaneNormal = GetActorForwardVector();
			FVector ProjectedCameraRight = FVector::VectorPlaneProject(CameraRight, ClimbingPlaneNormal).GetSafeNormal();

			FVector ClimbingRightMovement = ProjectedCameraRight * MoveVector.X;
			ClimbingRightMovement = ClimbingRightMovement.GetClampedToMaxSize(1.0f);

			//Forward movement math
			FVector ClimbDirection = (GetActorUpVector() * MoveVector.Y).GetClampedToMaxSize(1.0f);

			AddMovementInput(ClimbingRightMovement * 0.3f);
			AddMovementInput(ClimbDirection, 1.0f * 0.3f);
		}
	}
}


void AGP3CharacterMovement::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
	}
}

void AGP3CharacterMovement::isJumping()
{
	Jumping = !Jumping;
	IsGrounded = false;
	IsAirborne = true;
}

void AGP3CharacterMovement::Zoom(const FInputActionValue& Value)
{
	float value = Value.Get<float>();

	if (value)
	{
		float temp = camArm->TargetArmLength + (value * -10);
		if (ESizeState::Small == SizeState)
		{
			if (temp < CamLengthNormal && temp > CamLengthChanged)
			{
				camArm->TargetArmLength = temp;
			}
		}
		else if (ESizeState::Big == SizeState)
		{
			float CamlenghtSum = CamLengthChanged - CamLengthNormal;
			if (temp < (CamlenghtSum + CamLengthChanged) && temp > CamLengthChanged)
			{
				camArm->TargetArmLength = temp;
			}
		}
		else if (ESizeState::Normal == ESizeState::Normal)
		{
			if (temp < CamLengthChanged && temp > CamLengthNormal)
			{
				camArm->TargetArmLength = temp;
			}
		}
	}
}

void AGP3CharacterMovement::Sprint()
{
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		isWalking = false;
	}
}

void AGP3CharacterMovement::StopSprint()
{
	isWalking = true;
}

void AGP3CharacterMovement::CanRampSlide()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		FFindFloorResult FloorResult = GetCharacterMovement()->CurrentFloor;
		FHitResult HitResult = FloorResult.HitResult;

		float SlopeAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(HitResult.ImpactNormal, FVector::UpVector)));

		if (SlopeAngle > SlopeAngleThreshold && !isClimbing && !isClimbingUp) // Remove && to make us unable to climb upside down
		{
			IsOnSteepSlope = true;
			FTimerHandle SlideTimerHandle;
			GetWorldTimerManager().SetTimer(SlideTimerHandle, this, &AGP3CharacterMovement::ResetSlide, 0.1f, false); // Change here to fix sliding
			GetCharacterMovement()->SetWalkableFloorAngle(1.0f);
			FRotator NewRotation = UKismetMathLibrary::MakeRotFromZX(HitResult.ImpactNormal, GetActorForwardVector());
			//SetActorRotation(NewRotation); // with this enabled the climb jumping is all kinds of messed up // temp fix
			IsSliding = true;
			GetCharacterMovement()->bOrientRotationToMovement = false;
		}
		else
		{
			IsOnSteepSlope = false;
			GetCharacterMovement()->SetWalkableFloorAngle(44.0f);

			if (!GetCharacterMovement()->IsFalling())
			{
				//IsSliding = false;
				GetCharacterMovement()->bOrientRotationToMovement = true;
			}
		}
		if (!IsOnSteepSlope)
		{
			IsSliding = false;
		}
	}
}

void AGP3CharacterMovement::ResetSlide()
{
	//IsSliding = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AGP3CharacterMovement::HandleClimbing()
{
	FVector StartLocation = GetCapsuleComponent()->GetComponentLocation();
	FVector ForwardVector = GetCapsuleComponent()->GetForwardVector();
	FVector HorizontalEndLocation = StartLocation + (ForwardVector * 80.0f);

	FHitResult HorizontalHitResult;
	FCollisionQueryParams CollisionParams;

	DrawDebugLine(GetWorld(), StartLocation, HorizontalEndLocation, FColor::Red, false, 1, 0, 1); // Same as vertical

	if (GetWorld()->LineTraceSingleByChannel(HorizontalHitResult, StartLocation, HorizontalEndLocation, ECC_GameTraceChannel2, CollisionParams))
	{
		if (HorizontalHitResult.bBlockingHit)
		{
			AActor* HitActor = HorizontalHitResult.GetActor();
			FVector HitLocation = HorizontalHitResult.ImpactPoint;
			if(Dropped && HitActor == LatestHitActor)
			{
				return;
			}
			if (HitActor && HitActor->ActorHasTag("Climbable"))
			{
				Dropped = false;
				LatestHitActor = HorizontalHitResult.GetActor();
				FVector VerticalStartLocation = HitLocation + FVector(ForwardVector.X * 10.0f, ForwardVector.Y * 10.0f, 1.0f);
				FVector VerticalEndLocation = VerticalStartLocation + FVector(0, 0, 80.0f);

				FHitResult VerticalHitResult;

				if (GetWorld()->LineTraceSingleByChannel(VerticalHitResult, VerticalStartLocation, VerticalEndLocation, ECC_GameTraceChannel2, CollisionParams))
				{
					if (VerticalHitResult.bBlockingHit)
					{
						LedgeLocation = VerticalHitResult.ImpactPoint;
						isClimbing = true;
						CurrentClimbableActor = HitActor;
						GetCharacterMovement()->SetMovementMode(MOVE_Flying); // Handles moving when attached
						GetCharacterMovement()->bOrientRotationToMovement = false;
						GetCharacterMovement()->BrakingFrictionFactor = 50.0f;

						FVector ClimbSurfaceNormal = HorizontalHitResult.ImpactNormal;
						FRotator NewRotation = UKismetMathLibrary::MakeRotFromXZ(ClimbSurfaceNormal * -1, FVector::UpVector);
						SetActorRotation(NewRotation);
					}
				}
			}
		}
	}
	else
	{
		if (isClimbing)
		{
			isClimbing = false;
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
		}
	}
}



void AGP3CharacterMovement::HandleClimbingEdges()
{
	FVector CharacterLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();
	FVector UpVector = GetActorUpVector();

	FCollisionQueryParams CollisionParams;

	FVector LeftCheckStart = CharacterLocation - (RightVector * 30.0f);
	FVector LeftCheckEnd = LeftCheckStart + (ForwardVector * 80.0f);

	FHitResult LeftHitResult;
	bool bLeftHit = GetWorld()->LineTraceSingleByChannel(LeftHitResult, LeftCheckStart, LeftCheckEnd, ECC_GameTraceChannel2, CollisionParams);

	FVector RightCheckStart = CharacterLocation + (RightVector * 30.0f);
	FVector RightCheckEnd = RightCheckStart + (ForwardVector * 80.0f);

	FHitResult RightHitResult;
	bool bRightHit = GetWorld()->LineTraceSingleByChannel(RightHitResult, RightCheckStart, RightCheckEnd, ECC_GameTraceChannel2, CollisionParams);

	FVector DownCheckStart = CharacterLocation + (UpVector * -30.0f);
	FVector DownCheckEnd = DownCheckStart + (ForwardVector * 80.0f);

	FHitResult DownHitResult;
	bool bDownHit = GetWorld()->LineTraceSingleByChannel(DownHitResult, DownCheckStart, DownCheckEnd, ECC_GameTraceChannel2, CollisionParams);

	DrawDebugLine(GetWorld(), LeftCheckStart, LeftCheckEnd, FColor::Green, false, 1, 0, 1);
	DrawDebugLine(GetWorld(), RightCheckStart, RightCheckEnd, FColor::Green, false, 1, 0, 1);
	DrawDebugLine(GetWorld(), DownCheckStart, DownCheckEnd, FColor::Green, false, 1, 0, 1);

	if (!bLeftHit)
	{ 
		if (MoveVector.X < 0)
		{
			MoveVector.X = 0;
		}
	}
	if (!bRightHit)
	{
		if (MoveVector.X > 0)
		{
			MoveVector.X = 0;
		}
	}
	if (!bDownHit) 
	{
		if (MoveVector.Y < 0)
		{
			MoveVector.Y = 0;
		}
	}
}

void AGP3CharacterMovement::LedgeMantle()
{
	FVector UpCheckStart = GetActorLocation() + (GetActorUpVector() * 50.0f);
	FVector UpCheckEnd = UpCheckStart + (GetActorForwardVector() * 100.0f);

	FHitResult UpHitResult;
	bool bUpHit = GetWorld()->LineTraceSingleByChannel(UpHitResult, UpCheckStart, UpCheckEnd, ECC_GameTraceChannel2, FCollisionQueryParams());
	FVector NewLocation = LedgeLocation + FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

	TArray<FHitResult> Hits; 
	FCollisionShape ColShape = FCollisionShape::MakeSphere(40);
	FCollisionQueryParams CollisionParams;
	bool IsBlocked = GetWorld()->SweepMultiByChannel(Hits, NewLocation, NewLocation, GetActorQuat(), ECC_GameTraceChannel5, ColShape, CollisionParams);
	DrawDebugSphere(GetWorld(), NewLocation, ColShape.GetSphereRadius(), 24, FColor::Cyan, false, 5);
	for( FHitResult hit : Hits)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Orange, hit.GetActor()->GetName());
	}
	if (IsBlocked)
	{
		return;
	}
	/////////////////
	if (isClimbing && !bUpHit)
	{
		isClimbingUp = true;
		isClimbing = false;

		GetCharacterMovement()->DisableMovement();

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AGP3CharacterMovement::ResetClimbingUp, 2.04f, false);
	}
}

void AGP3CharacterMovement::ResetClimbingUp()
{
	isClimbingUp = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AGP3CharacterMovement::HandleDrop()
{
	if (!IsGrounded && IsAirborne && isClimbing)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Magenta, "Tihi, I have been pressed");
		Dropped = true;
		isClimbing = false;
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
	}
	
}

void AGP3CharacterMovement::CheckForClimbableJump()
{
	if (bClimbJumping)
		return;

	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();
	FVector UpVector = GetActorUpVector();
	
	TArray<VectorArray> CheckArray =
	{
		VectorArray((ForwardVector + UpVector).GetSafeNormal(), UpVector),
		VectorArray((ForwardVector + RightVector).GetSafeNormal(), RightVector),
		VectorArray((ForwardVector - RightVector).GetSafeNormal(), -RightVector),
		VectorArray((ForwardVector + RightVector + UpVector).GetSafeNormal(), (RightVector + UpVector).GetSafeNormal()),
		VectorArray((ForwardVector - RightVector + UpVector).GetSafeNormal(), (-RightVector + UpVector).GetSafeNormal())
	};

	for (const VectorArray& Array : CheckArray)
	{
		FVector StartLocation = GetActorLocation();
		FVector Direction = Array.CheckDirection;
		FVector EndLocation = StartLocation + (Direction * WallClimbJumpDistance);

		//DEBUG
		FVector Startn = GetActorLocation();
		FVector Dir = Array.JumpDirection;
		FVector End = StartLocation + (Dir * WallClimbJumpForce);

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;

		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1, 0, 1);
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_GameTraceChannel2, CollisionParams))
		{
			DrawDebugBox(GetWorld(), HitResult.ImpactPoint, FVector(1, 1, 1), FColor::Blue, false, 3.f);
			if (HitResult.bBlockingHit)
			{
				AActor* HitActor = HitResult.GetActor();
				if (HitActor && HitActor->ActorHasTag("Climbable") && HitActor != CurrentClimbableActor)
				{
					if (HitResult.ImpactPoint != CurrentClimbableLocation)
					{
						isClimbing = false;
						GetCharacterMovement()->SetMovementMode(MOVE_Walking);
						GetCharacterMovement()->bOrientRotationToMovement = true;
						GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
						ClimbJumpTarget = HitResult.ImpactPoint;
						FVector JumpDir = Array.JumpDirection;
						PerformClimbJump(JumpDir);
						DrawDebugLine(GetWorld(), Startn, End, FColor::Black, false, 5, 0, 1);
						break;
					}
				}
			}
		}
	}
}


void AGP3CharacterMovement::PerformClimbJump(FVector JumpDirection)
{
	bClimbJumping = true;

	FVector LaunchVelocity = JumpDirection * WallClimbJumpForce;

	LaunchCharacter(LaunchVelocity, true, true);

	isClimbing = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
	FTimerHandle ClimbJumpTimerHandle;
	GetWorldTimerManager().SetTimer(ClimbJumpTimerHandle, this, &AGP3CharacterMovement::FindNewClimbingPoint, 0.1f, false);
}

void AGP3CharacterMovement::FindNewClimbingPoint()
{
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = ClimbJumpTarget;

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;

	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_GameTraceChannel2, CollisionParams))
	{
		if (HitResult.bBlockingHit)
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor && HitActor->ActorHasTag("Climbable") && HitActor != CurrentClimbableActor)
			{
				FVector TargetLocation = HitResult.ImpactPoint;

				FVector LeftCheckStart = TargetLocation - (GetActorRightVector() * 50.0f);
				FVector LeftCheckEnd = LeftCheckStart + FVector(0, 0, 200.0f);

				FHitResult LeftHitResult;
				bool bLeftHit = GetWorld()->LineTraceSingleByChannel(LeftHitResult, LeftCheckStart, LeftCheckEnd, ECC_GameTraceChannel2, CollisionParams);

				FVector RightCheckStart = TargetLocation + (GetActorRightVector() * 50.0f);
				FVector RightCheckEnd = RightCheckStart + FVector(0, 0, 200.0f);

				FHitResult RightHitResult;
				bool bRightHit = GetWorld()->LineTraceSingleByChannel(RightHitResult, RightCheckStart, RightCheckEnd, ECC_GameTraceChannel2, CollisionParams);

				if (bLeftHit && bRightHit)
				{
					FVector AdjustedLocation = TargetLocation - (HitResult.ImpactNormal * GetCapsuleComponent()->GetScaledCapsuleRadius());

					FVector CapsuleHalfHeight(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
					FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(GetCapsuleComponent()->GetScaledCapsuleRadius(), CapsuleHalfHeight.Z);

					bool bCanFit = !GetWorld()->SweepSingleByChannel(
						HitResult,
						AdjustedLocation + CapsuleHalfHeight,
						AdjustedLocation + CapsuleHalfHeight,
						FQuat::Identity,
						ECC_GameTraceChannel2,
						CollisionShape,
						CollisionParams
					);

					if (bCanFit)
					{
						GetCharacterMovement()->StopMovementImmediately();
						SetActorLocation(AdjustedLocation);
						GetCharacterMovement()->SetMovementMode(MOVE_Flying);
						isClimbing = true;
						CurrentClimbableLocation = HitResult.ImpactPoint;
						CurrentClimbableActor = HitActor;
					}
				}
			}
		}
	}

	bClimbJumping = false;
}

void AGP3CharacterMovement::UpdateClimbingRotation()
{
	FVector StartLocation = GetCapsuleComponent()->GetComponentLocation();
	FVector ForwardVector = GetCapsuleComponent()->GetForwardVector();
	FVector HorizontalEndLocation = StartLocation + (ForwardVector * 100.0f);

	FHitResult HorizontalHitResult;
	FCollisionQueryParams CollisionParams;

	if (GetWorld()->LineTraceSingleByChannel(HorizontalHitResult, StartLocation, HorizontalEndLocation, ECC_GameTraceChannel2, CollisionParams))
	{
		if (HorizontalHitResult.bBlockingHit)
		{
			FVector ClimbSurfaceNormal = HorizontalHitResult.ImpactNormal;
			FRotator NewRotation = UKismetMathLibrary::MakeRotFromXZ(ClimbSurfaceNormal * -1, FVector::UpVector);
			SetActorRotation(NewRotation);
		}
	}
}