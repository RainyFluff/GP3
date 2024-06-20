#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "GP3Teleporter.generated.h"

UCLASS()
class AGP3Teleporter : public ATriggerBox
{
	GENERATED_BODY()

public:

	AGP3Teleporter();

	//virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void EnterTeleporter(class AActor* overlappedActor, class AActor* otherActor);

	UFUNCTION()
	void ExitTeleporter(class AActor* overlappedActor, class AActor* otherActor);

	UPROPERTY(EditAnywhere, Category = "Teleporter")
	AGP3Teleporter* TeleportTo;

	UPROPERTY()
	bool isTeleporting;

};