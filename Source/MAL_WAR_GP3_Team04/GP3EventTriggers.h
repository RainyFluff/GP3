#pragma once
#include "GameFramework/Actor.h"
#include "GP3BakerDetection.h"
#include "Components/BoxComponent.h"
#include "GP3EventTriggers.generated.h"

class ACharacter;

UCLASS()
class AGP3Event : public AActor
{
	GENERATED_BODY()
public:
	AGP3Event();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly)
	float EventLookTime;

	UPROPERTY(EditInstanceOnly)
	float EventLookAwayTime;

	UPROPERTY(EditInstanceOnly)
	EBakerState NewBakerState;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* FeetPos = nullptr;
	FQuat FeetRotation;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Collider = nullptr;

	UPROPERTY(EditAnywhere)
	ABaker* Baker = nullptr;

	UPROPERTY(VisibleAnywhere)
	bool HasBeenTriggered;

	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};

