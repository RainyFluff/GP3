#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GP3BakerDetection.h"
#include "GP3BakerAnimEvent.generated.h"

UCLASS()
class AGP3BakerAnimEvent : public AActor
{
	GENERATED_BODY()
	
public:	
	AGP3BakerAnimEvent();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayMontage();

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	UAnimMontage* AnimMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ABaker* Baker = nullptr;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root = nullptr;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Collider = nullptr;

	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
