// Copyright Epic Games, Inc. All Rights Reserved.

#include "GP3HomeWorkGameMode.h"
#include "GP3HomeWorkCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGP3HomeWorkGameMode::AGP3HomeWorkGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
