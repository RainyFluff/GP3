// Copyright Epic Games, Inc. All Rights Reserved.

#include "MAL_WAR_GP3_Team04GameMode.h"
#include "MAL_WAR_GP3_Team04Character.h"
#include "UObject/ConstructorHelpers.h"

AMAL_WAR_GP3_Team04GameMode::AMAL_WAR_GP3_Team04GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
