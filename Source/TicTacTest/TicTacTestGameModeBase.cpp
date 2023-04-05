
#include "TicTacTestGameModeBase.h"
#include "TicTacBoard.h"
#include "TicTacController.h"

#include "Blueprint/UserWidget.h"
#include "UMG.h"

ATicTacTestGameModeBase::ATicTacTestGameModeBase()
{
	DefaultPawnClass = ATicTacPawn::StaticClass();
	PlayerControllerClass = ATicTacController::StaticClass();  
}

void ATicTacTestGameModeBase::BeginPlay()
{
  Super::BeginPlay();
  InitializeGameMenus();

  APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
  TicTacPawn = dynamic_cast<ATicTacPawn*>(PlayerController->GetPawn());

  if (TicTacPawn)
  {
    TicTacPawn->OnGameFinished.BindUObject(this, &ATicTacTestGameModeBase::OnGameFinished);
  }
}

void ATicTacTestGameModeBase::InitializeGameMenus()
{
  check(TicTacStartGameMenu != nullptr || TicTacEndGameMenu != nullptr);
  if (TicTacStartGameMenu == nullptr || TicTacEndGameMenu == nullptr)
  {
    UE_LOG(LogTemp, Display, TEXT("TicTacGameMenu is not initialized!"));
    return;
  }
  
  MainGameMenu = CreateWidget<UUserWidget>(GetWorld(), TicTacStartGameMenu);
  MainGameMenu->AddToPlayerScreen();

  EndGameMenu = CreateWidget<UUserWidget>(GetWorld(), TicTacEndGameMenu);
  EndGameMenu->AddToPlayerScreen();

  HideGameMenu(EndGameMenu);
}

void ATicTacTestGameModeBase::StartNewGame()
{
  HideGameMenu(MainGameMenu);
}

void ATicTacTestGameModeBase::RestartGameMenus()
{
  HideGameMenu(EndGameMenu);
  ShowGameMenu(MainGameMenu);
}

inline void ATicTacTestGameModeBase::ShowGameMenu(UUserWidget* gameMenu)
{
  if (gameMenu != nullptr && !gameMenu->IsVisible())
  {
    gameMenu->SetVisibility(ESlateVisibility::Visible);
  }
}

inline void ATicTacTestGameModeBase::HideGameMenu(UUserWidget* gameMenu)
{
  if (gameMenu != nullptr && gameMenu->IsVisible())
  {
    gameMenu->SetVisibility(ESlateVisibility::Hidden);
  }
}

void ATicTacTestGameModeBase::OnGameFinished(WinCondition gameResult)
{
  if (EndGameMenu == nullptr && !EndGameMenu->GetIsEnabled())
  {
    UE_LOG(LogTemp, Display, TEXT("TicTacEndGameMenu is nullptr!"));
    return;
  }

  // Get a reference to the widget's GameResult variable
  UTextBlock* GameResultText = Cast<UTextBlock>(EndGameMenu->GetWidgetFromName("GameResultText"));

  // Set the text of the GameResult variable based on the game result
  if (GameResultText != nullptr)
  {
    if (gameResult == WinCondition::Player1Win)
    {
      GameResultText->SetText(FText::FromString("Player 1 win!"));
    }
    else if (gameResult == WinCondition::Player2Win)
    {
      GameResultText->SetText(FText::FromString("Player 2 win!"));
    }
    else
    {
      GameResultText->SetText(FText::FromString("Draw!"));
    }
  }

  ShowGameMenu(EndGameMenu);
}
