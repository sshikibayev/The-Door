// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheDoorGameMode.h"
#include "TheDoorCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATheDoorGameMode::ATheDoorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
