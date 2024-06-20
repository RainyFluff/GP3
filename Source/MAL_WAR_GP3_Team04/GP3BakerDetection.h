#pragma once
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h" // Needed to convert character to an actor
#include "GP3BakerDetection.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBakerDetectionSignature);

UENUM(BlueprintType)
enum class EBakerState : uint8 // Makes the enum smaller - max size of enum 256 elements (we will never reach that)
{
	EIdle,
	ENotLooking,
	ELooking,
	EKill
};

UCLASS()
class ABaker : public AActor
{
	GENERATED_BODY()

public:
	ABaker();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void UpdateBakerAIState(EBakerState newState);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EBakerState AIState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* HitActor = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float LookTime;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float LookAwayTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* Player = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Character;

	UFUNCTION(BlueprintCallable)
	void UpdateBaker(float templooktime, float templookawaytime, EBakerState newai, FVector newlocation, FQuat newrotation);

private:
	UPROPERTY(BlueprintAssignable)
	FBakerDetectionSignature OnLook;

	UPROPERTY(BlueprintAssignable)
	FBakerDetectionSignature OnKill;

	UPROPERTY(BlueprintAssignable)
	FBakerDetectionSignature OnNotLook;

	UPROPERTY(BlueprintAssignable)
	FBakerDetectionSignature OnIdle;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root = nullptr;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Eyes = nullptr;

	UPROPERTY(VisibleAnywhere)
	ACharacter* PlayerChar = nullptr;

	UPROPERTY(VisibleAnywhere)
	FVector LinetraceLoc;
};