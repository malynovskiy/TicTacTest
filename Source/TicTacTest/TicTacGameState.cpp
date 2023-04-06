#include "TicTacGameState.h"

void ATicTacGameState::SwitchPlayer()
{
  CurrentPlayer = CurrentPlayer == EPlayer::Player1 ? EPlayer::Player2 : EPlayer::Player1;
}

bool ATicTacGameState::IsAITurn() const
{
  return CurrentGameMode == EGameMode::PVE && CurrentPlayer == EPlayer::Player2;
}

bool ATicTacGameState::IsPlayerTurn() const
{
  return CurrentGameMode == EGameMode::PVE && CurrentPlayer == EPlayer::Player1;
}

void ATicTacGameState::OnGameStateChanged(EGameState NewGameState)
{
  if(NewGameState != EGameState::NoWin)
    OnGameFinished.ExecuteIfBound(NewGameState);
}
