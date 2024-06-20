#pragma once
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GP3CharacterMovement.h"
#include "GP3MovableObject.generated.h"

UCLASS()
class AMovableObject : public AActor
{
	GENERATED_BODY()

public:
	AMovableObject();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UStaticMeshComponent* Mesh = nullptr;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root = nullptr;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* YCollider = nullptr;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* XCollider = nullptr;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Point = nullptr;

	UPROPERTY(VisibleAnywhere)
	bool CanBeInteractedWith;

	UPROPERTY(EditAnywhere)
	float groundCheckDistance = 100.f;

	UPROPERTY(VisibleAnywhere)
	AActor* Player = nullptr;

};