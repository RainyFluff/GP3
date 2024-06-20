#include "GP3TriggerVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraShake.h"
#include "GP3CharacterMovement.h"
#include "GP3TriggerShake.h"

AGP3TriggerVolume::AGP3TriggerVolume()
{
    PrimaryActorTick.bCanEverTick = true;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;

    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGP3TriggerVolume::OnOverlapBegin);
}

void AGP3TriggerVolume::BeginPlay()
{
    Super::BeginPlay();
}

void AGP3TriggerVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && OtherComp && OtherActor->IsA(AGP3CharacterMovement::StaticClass()))
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
        if (PlayerController)
        {
            PlayerController->ClientStartCameraShake(UGP3TriggerShake::StaticClass());
        }

        Destroy();
    }
}