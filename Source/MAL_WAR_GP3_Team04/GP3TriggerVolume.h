#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP3TriggerVolume.generated.h"

UCLASS()
class AGP3TriggerVolume : public AActor
{
    GENERATED_BODY()

public:
    AGP3TriggerVolume();

protected:
    virtual void BeginPlay() override;

public:
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Trigger")
    class UBoxComponent* TriggerBox;

    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};