#include "GP3EventTriggers.h"

AGP3Event::AGP3Event()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
	Collider = CreateDefaultSubobject<UBoxComponent>("Collider");
	Collider->SetupAttachment(Root);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AGP3Event::HandleBeginOverlap);

	PrimaryActorTick.bCanEverTick = true;
}

void AGP3Event::BeginPlay()
{
	Super::BeginPlay();
}

void AGP3Event::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGP3Event::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasBeenTriggered && OtherActor->IsA<ACharacter>())
	{
		FVector Location;
		if (FeetPos != nullptr)
		{
			Location = FeetPos->GetActorLocation();
			FeetRotation = FeetPos->GetActorQuat();
		}
		else
		{
			Location = Baker->GetActorLocation();
			FeetRotation = Baker->GetActorQuat();
		}
		HasBeenTriggered = true;
		Baker->UpdateBaker(EventLookTime, EventLookAwayTime, NewBakerState, Location, FeetRotation);
	}
}
