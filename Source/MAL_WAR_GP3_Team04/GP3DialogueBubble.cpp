#include "GP3DialogueBubble.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/TextRenderComponent.h"
#include "UserSettings/EnhancedInputUserSettings.h" // included for later addon (if theres time)

AGP3DialogueBubble::AGP3DialogueBubble()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	Sphere = CreateDefaultSubobject <USphereComponent>("Sphere");
	Sphere->SetupAttachment(Root);
	Sphere->SetSphereRadius(1000.f);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AGP3DialogueBubble::HandleBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AGP3DialogueBubble::HandleEndOverlap);

	YawRoot = CreateDefaultSubobject<USceneComponent>("YawRoot");
	YawRoot->SetupAttachment(Root);

	Text = CreateDefaultSubobject<UTextRenderComponent>("Text");
	Text->SetupAttachment(YawRoot);

	PrimaryActorTick.bCanEverTick = true;
}

void AGP3DialogueBubble::BeginPlay()
{
	Super::BeginPlay();
}

void AGP3DialogueBubble::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target)
	{
		FVector TargetDirection = (Target->GetActorLocation() - YawRoot->GetComponentLocation()).GetSafeNormal();
		FRotator TargetRotation = UKismetMathLibrary::MakeRotFromX(TargetDirection);
		YawRoot->SetWorldRotation(FRotator(0.f, TargetRotation.Yaw, 0.f));
	}
}


void AGP3DialogueBubble::HandleBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	ActivateText();
	if (OtherActor->IsA<ACharacter>())
	{
		Target = OtherActor;
	}
}

void AGP3DialogueBubble::HandleEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	DeactivateText();
	if (OtherActor == Target)
	{
		Target = nullptr;
	}
}

