#include "GP3Teleporter.h"
#include "GP3CharacterMovement.h"
#include "DrawDebugHelpers.h"


AGP3Teleporter::AGP3Teleporter()
{
	OnActorBeginOverlap.AddDynamic(this, &AGP3Teleporter::EnterTeleporter);
	OnActorEndOverlap.AddDynamic(this, &AGP3Teleporter::ExitTeleporter);

	isTeleporting = false;
	PrimaryActorTick.bCanEverTick = true;
}



void AGP3Teleporter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugBox(GetWorld(), GetActorLocation(), GetActorScale(), FColor::Magenta, false, 0.1f); // pissy but cant get it working
}

void AGP3Teleporter::EnterTeleporter(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor && otherActor != this)
	{
		if (TeleportTo)
		{
			AGP3CharacterMovement* character = Cast<AGP3CharacterMovement>(otherActor);
			if (character && !TeleportTo->isTeleporting)
			{
				isTeleporting = true;
				character->SetActorRotation(TeleportTo->GetActorRotation());
				character->GetController()->SetControlRotation(character->GetActorRotation());
				character->SetActorLocation(TeleportTo->GetActorLocation());
			}
		}

	}
}

void AGP3Teleporter::ExitTeleporter(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor && otherActor != this)
	{
		if (TeleportTo && !isTeleporting)
		{
			TeleportTo->isTeleporting = false;
		}
	}
}