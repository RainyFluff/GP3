#include "GP3BakerAnimEvent.h"

AGP3BakerAnimEvent::AGP3BakerAnimEvent()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
	Collider = CreateDefaultSubobject<UBoxComponent>("Collider");
	Collider->SetupAttachment(Root);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AGP3BakerAnimEvent::HandleBeginOverlap);

	PrimaryActorTick.bCanEverTick = true;
}

void AGP3BakerAnimEvent::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGP3BakerAnimEvent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGP3BakerAnimEvent::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayMontage();
}

