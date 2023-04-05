#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TicTacGameState.generated.h"

namespace TicTacTest
{
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
}

DECLARE_DELEGATE_OneParam(FOnGameFinished, TicTacTest::EGameState);

UCLASS()
class TICTACTEST_API ATicTacGameState : public AGameStateBase
{
  GENERATED_BODY()

public:
  using EPlayer = TicTacTest::EPlayer;
  using EGameState = TicTacTest::EGameState;
  using EGameMode = TicTacTest::EGameMode;

  FOnGameFinished OnGameFinished;

  void SwitchPlayer();

  inline EGameState GetGameState() const { return CurrentGameState; }
  inline void SetGameState(EGameState NewGameState) { CurrentGameState = NewGameState; OnGameStateChanged(NewGameState); }

  inline EGameMode GetGameMode() const { return CurrentGameMode; }
  inline void SetGameMode(EGameMode NewGameMode) { CurrentGameMode = NewGameMode; }

  inline EPlayer GetCurrentPlayer() const { return CurrentPlayer; }
  //inline void SetCurrentPlayer(Player ) { CurrentPlayer; }

private:
  void OnGameStateChanged(EGameState NewGameState);

protected:
  //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TicTacTest")
  EPlayer CurrentPlayer = EPlayer::Player1;

  //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TicTacTest")
  EGameState CurrentGameState = EGameState::NoWin;

  //UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TicTacTest")
  EGameMode CurrentGameMode = EGameMode::PVP;
};
