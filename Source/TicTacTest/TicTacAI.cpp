#include "TicTacAI.h"
#include "TicTacBoard.h"
#include "TicTacBlock.h"

ATicTacAI::ATicTacAI()
{
  PrimaryActorTick.bCanEverTick = true;
}

void ATicTacAI::BeginPlay()
{
  Super::BeginPlay();

  GameState = GetWorld()->GetGameState<ATicTacGameState>();

  if (ATicTacPawn* ticTacPawn = dynamic_cast<ATicTacPawn*>(GetWorld()->GetFirstPlayerController()->GetPawn()))
  {
    GameBoard = ticTacPawn->GetGameBoard();
  }

  float CheckInterval = 1.0f;
  FTimerHandle TimerHandle;
  GetWorldTimerManager().SetTimer(TimerHandle, this, &ATicTacAI::CheckTurn, CheckInterval, true);
}

void ATicTacAI::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

int32 ATicTacAI::ComputeNextMove(TArray<int32> possibleMoves)
{
  if (possibleMoves.Num() == 0)
  {
    UE_LOG(LogTemp, Warning, TEXT("GetRandomIndexFromArray: The input array is empty!"));
    return -1;
  }

  // Generate a random index
  int32 RandomIndex = FMath::RandRange(0, possibleMoves.Num() - 1);

  // Return the value at the randomly generated index
  return possibleMoves[RandomIndex];
}

void ATicTacAI::CheckTurn()
{
  if (BlockToMove == nullptr && GameState && GameState->IsAITurn())
  {
    const int32 cellIndex = ComputeNextMove(GameBoard->GetEmptyCells());

    BlockToMove = GameBoard->GetBlock(cellIndex);
    BlockToMove->Highlight(true);

    float moveDelay = 1.0f;
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, this, &ATicTacAI::MakeMove, moveDelay, false);
  }
}

void ATicTacAI::MakeMove()
{
  if (BlockToMove != nullptr)
  {
    BlockToMove->HandleClicked(EPlayer::Player2);
    BlockToMove = nullptr;
  }
}

