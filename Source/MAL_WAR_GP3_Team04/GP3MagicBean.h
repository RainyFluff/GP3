#pragma once

#include "GameFramework/Actor.h"
#include "GP3CharacterMovement.h"
#include "GP3MagicBean.generated.h"



class USceneComponent;
class USphereComponent;
class AGP3CharacterMovement;


UCLASS()
class AMagicBean : public AActor
{
	GENERATED_BODY()

public:
	AMagicBean();

	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable)
	void UpdateSizeStates(AGP3CharacterMovement* charMov, ESizeState State, FVector& returnValue);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bean Values",  meta = (ClampMin = "0.0", ClampMax = "3000.0"))
	float BeanSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bean Values", meta = (ClampMin = "0.0", ClampMax = "10000.0"))
	float BeanJump;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bean Values", meta = (ClampMin = "0.0", ClampMax = "10000000.0"))
	float PushForce;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bean Values", meta = (ClampMin = "0.0", ClampMax = "10000.0"))
	float ChangedMass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bean Values", meta = (ClampMin = "0.0", ClampMax = "10000.0"))
	float ChangedLengthCam;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool IsGrowingBean;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool IsShrinkingBean;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	AActor* Character = nullptr;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FVector CharaSize;

	UPROPERTY(BlueprintReadWrite,  EditInstanceOnly)
	float SizeMultiplier = 2.5f;

protected:
	virtual void BeginPlay() override;

	/*
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	*/

private:

	void NormalValues(AGP3CharacterMovement* charMov);

	void BeanValues(AGP3CharacterMovement* charMov);

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* BeanCol;


	float NormalSpeed;

	float NormalJumpVel;

	float NormalPushForce;

	float NormalMass;

	float NormalCamLength;
};