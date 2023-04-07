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

  float CheckInterval = 0.5f;
  FTimerHandle TimerHandle;
  GetWorldTimerManager().SetTimer(TimerHandle, this, &ATicTacAI::CheckTurnAndMove, CheckInterval, true);
}

void ATicTacAI::SetGameBoard(ATicTacBoard* board)
{
  GameBoard = board;
}

int32 ATicTacAI::GetRandMove(TArray<int32> possibleMoves)
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

void ATicTacAI::CheckTurnAndMove()
{
  if (GameState == nullptr || !GameState->IsAITurn(Player))
    return;

  const int32 cellIndex = GetNextMove();
  ATicTacBlock *BlockToMove = GameBoard->GetBlock(cellIndex);
  BlockToMove->HandleMove(Player);
}

int32 ATicTacAI::GetNextMove()
{
  int32 index = 0;

  // Find a winning move or a blocking move
  for (int32 i = 0; i < GameBoard->Size; ++i)
  {
    for (int32 j = 0; j < GameBoard->Size; ++j)
    {
      index = i * GameBoard->Size + j;
      if (!GameBoard->IsCellEmpty(index))
        continue;

      if (GameBoard->IsWinningMove(index, EPlayer::Player2))
        return index;

      if (GameBoard->IsWinningMove(index, EPlayer::Player1))
        return index;
    }
  }

  return GetRandMove(GameBoard->GetEmptyCells());
}

