#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TicTacGameState.generated.h"

UENUM(BlueprintType)
enum class EPlayer : uint8
{
  Player1,
  Player2
};
UENUM(BlueprintType)
enum class EGameState : uint8
{
  NoWin,
  Player1Win,
  Player2Win,
  Draw
};

UENUM(BlueprintType)
enum class EGameMode : uint8
{
  PVP,
  PVE
};

DECLARE_DELEGATE_OneParam(FOnGameFinished, EGameState);
DECLARE_DELEGATE_OneParam(FOnPlayerChanged, EPlayer);

UCLASS()
class TICTACTEST_API ATicTacGameState : public AGameStateBase
{
  GENERATED_BODY()

public:
  FOnGameFinished OnGameFinished;
  FOnPlayerChanged OnPlayerChanged;

  void SwitchPlayer();

  inline EGameState GetGameState() const { return CurrentGameState; }
  inline void SetGameState(EGameState NewGameState) { CurrentGameState = NewGameState; OnGameStateChanged(NewGameState); }

  inline EGameMode GetGameMode() const { return CurrentGameMode; }
  inline void SetGameMode(EGameMode NewGameMode) { CurrentGameMode = NewGameMode; }

  inline EPlayer GetCurrentPlayer() const { return CurrentPlayer; }

  bool IsAITurn(EPlayer player) const;
  bool IsPlayerTurn() const;

private:
  void OnGameStateChanged(EGameState NewGameState);

protected:
  EPlayer CurrentPlayer = EPlayer::Player1;

  EGameState CurrentGameState = EGameState::NoWin;

  UPROPERTY(BlueprintReadWrite, Category = "Gameplay")
  EGameMode CurrentGameMode = EGameMode::PVP;
};
