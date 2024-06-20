#pragma once
#include "GameFramework/Actor.h"
#include "GP3DialogueBubble.generated.h"

class USphereComponent;
class UTextRenderComponent;

UCLASS()
class AGP3DialogueBubble : public AActor
{
	GENERATED_BODY()

public:
	AGP3DialogueBubble();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UTextRenderComponent* Text = nullptr;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintImplementableEvent, Category = "Activate Text Bubble")
	void ActivateText();

	UFUNCTION(BlueprintImplementableEvent, Category = "Deactivate Text Bubble")
	void DeactivateText();


private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root = nullptr;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere = nullptr;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* YawRoot = nullptr;

	UPROPERTY(EditInstanceOnly)
	AActor* Target = nullptr;

};


