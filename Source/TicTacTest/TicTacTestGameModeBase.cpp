
#include "TicTacTestGameModeBase.h"
#include "TicTacPawn.h"
#include "TicTacController.h"

#include "Blueprint/UserWidget.h"

ATicTacTestGameModeBase::ATicTacTestGameModeBase()
{
	DefaultPawnClass = ATicTacPawn::StaticClass();
	PlayerControllerClass = ATicTacController::StaticClass();  
}

void ATicTacTestGameModeBase::BeginPlay()
{
  Super::BeginPlay();
  InitializeGameMenu();
}

void ATicTacTestGameModeBase::StartNewGame()
{
  if (TicTacGameMenu == nullptr)
  {
    UE_LOG(LogTemp, Display, TEXT("TicTacGameMenu is not initialized!"));
    return;
  }

  if (GameMenu && GameMenu->IsInViewport())
  {
    GameMenu->RemoveFromViewport();
  }
}

void ATicTacTestGameModeBase::InitializeGameMenu()
{
  if (TicTacGameMenu == nullptr)
  {
    UE_LOG(LogTemp, Display, TEXT("TicTacGameMenu is not initialized!"));
    return;
  }
  
  GameMenu = CreateWidget<UUserWidget>(GetWorld(), TicTacGameMenu);
  if (GameMenu != nullptr)
  {
    GameMenu->AddToViewport();
  }
}
