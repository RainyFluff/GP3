#include "GP3MovableObject.h"
#include "DrawDebugHelpers.h"

AMovableObject::AMovableObject()
{
	//Default values
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(Root);

	XCollider = CreateDefaultSubobject<UBoxComponent>("XCollider");
	XCollider->SetupAttachment(Mesh);
	XCollider->OnComponentBeginOverlap.AddDynamic(this, &AMovableObject::HandleBeginOverlap);
	XCollider->OnComponentEndOverlap.AddDynamic(this, &AMovableObject::HandleEndOverlap);


	YCollider = CreateDefaultSubobject<UBoxComponent>("YCollider");
	YCollider->SetupAttachment(Mesh);
	YCollider->OnComponentBeginOverlap.AddDynamic(this, &AMovableObject::HandleBeginOverlap);
	YCollider->OnComponentEndOverlap.AddDynamic(this, &AMovableObject::HandleEndOverlap);

	PrimaryActorTick.bCanEverTick = true;
	CanBeInteractedWith = false;
}

void AMovableObject::BeginPlay()
{
	Super::BeginPlay();
}

void AMovableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//DrawDebugBox(GetWorld(), XCollider->GetComponentLocation(), XCollider->GetScaledBoxExtent(), FColor::Emerald, false, 0.1f); // Debug help
	//DrawDebugBox(GetWorld(), YCollider->GetComponentLocation(), YCollider->GetScaledBoxExtent(), FColor::Emerald, false, 0.1f); // Debug help

	FHitResult Hit;
	bool IsGrounded = GetWorld()->LineTraceSingleByChannel(Hit, Mesh->GetComponentLocation(), -Mesh->GetUpVector() * groundCheckDistance + Mesh->GetComponentLocation(), ECC_Visibility);
	DrawDebugLine(GetWorld(), Mesh->GetComponentLocation(), -Mesh->GetUpVector() * groundCheckDistance + Mesh->GetComponentLocation(), FColor::Green, false, 0.1f);
	if(CanBeInteractedWith || !IsGrounded)
	{
		Mesh->SetSimulatePhysics(true);
	}
	else if (!CanBeInteractedWith && IsGrounded)
	{
		Mesh->SetSimulatePhysics(false);
	}
	
}

void AMovableObject::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<ACharacter>())
	{
		Player = OtherActor;
		CanBeInteractedWith = true;
	}
}

void AMovableObject::HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == Player)
	{
		Player = nullptr;
		CanBeInteractedWith = false;
	}
}
