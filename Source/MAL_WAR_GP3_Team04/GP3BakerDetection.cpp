#include "GP3BakerDetection.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Having trouble getting the Broadcast to work properly.
// Unsure if my switch statement is doing anything at all.
// All else should work.

ABaker::ABaker()
{
	LookAwayTime = 5.f;
	LookTime = 5.f;
	PlayerChar = nullptr;
	
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
	
	Eyes = CreateDefaultSubobject<USceneComponent>("Eyes");
	Eyes->SetupAttachment(Root);
	
	Character = CreateDefaultSubobject<USkeletalMeshComponent>("Character");
	Character->SetupAttachment(Root);

	PrimaryActorTick.bCanEverTick = true;
}

void ABaker::BeginPlay()
{
	Super::BeginPlay();
	PlayerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	UpdateBakerAIState(EBakerState::EIdle);
}

void ABaker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (AIState == EBakerState::ELooking)
	{
		Player = PlayerChar;
		LinetraceLoc = Eyes->GetComponentLocation();
		FHitResult Hit;

		FVector Start = LinetraceLoc;
		FVector End = Player->GetActorLocation();

		FCollisionQueryParams TraceParams;
		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams); // Alpha - subject to change
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.1f); // DEBUG
		HitActor = Hit.GetActor();
		if(HitActor == Player)
		{
			UpdateBakerAIState(EBakerState::EKill);
			DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 3.f); // DEBUG
		}
	}
	else if(AIState == EBakerState::ENotLooking)
	{

	}
	else if(AIState == EBakerState::EKill)
	{
		
	}
}

void ABaker::UpdateBakerAIState(EBakerState newState)
{
	switch (newState)
	{
	case EBakerState::EIdle:
		OnIdle.Broadcast();
		break;
	case EBakerState::ENotLooking:
		OnNotLook.Broadcast();
		break;
	case EBakerState::ELooking:
		PlayerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		OnLook.Broadcast();
		break;
	case EBakerState::EKill:
		OnKill.Broadcast();
		break;
	default:
		break;
	}
	AIState = newState;
}

void ABaker::UpdateBaker(float templooktime, float templookawaytime, EBakerState newai, FVector newlocation, FQuat newrotation)
{
	if (templooktime != 0)
	{
		LookTime = templooktime;
	}
	if (templookawaytime != 0)
	{
		LookAwayTime = templookawaytime;
	}
	if(!newlocation.ContainsNaN() && !newrotation.ContainsNaN())
	{
		SetActorLocation(newlocation);
		SetActorRotation(newrotation);
	}
	UpdateBakerAIState(newai);
}

