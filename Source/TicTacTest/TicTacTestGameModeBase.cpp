// Copyright Epic Games, Inc. All Rights Reserved.


#include "TicTacTestGameModeBase.h"
#include "TicTacPawn.h"
#include "TicTacController.h"

ATicTacTestGameModeBase::ATicTacTestGameModeBase()
{
	DefaultPawnClass = ATicTacPawn::StaticClass();
	PlayerControllerClass = ATicTacController::StaticClass();
}
