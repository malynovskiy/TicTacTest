#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TicTacPawn.generated.h"

namespace TicTacTest
{
	enum class Player : int32
	{
		Player1,
		Player2
	};

	enum class WinCondition : int32
	{
    NoWin,
    Player1Win,
    Player2Win,
    Draw
  };
}

DECLARE_DELEGATE_OneParam(FOnGameFinished, TicTacTest::WinCondition);

UCLASS()
class TICTACTEST_API ATicTacPawn : public APawn
{
	GENERATED_BODY()

	using Player = TicTacTest::Player;
	using WinCondition = TicTacTest::WinCondition;

public:

	ATicTacPawn();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  	FOnGameFinished OnGameFinished;

	UFUNCTION(BlueprintCallable, Category = "TicTacTest")
  	void StartNewGame();

	void EndGame(WinCondition gameResult);
	
	Player GetCurrentPlayer() const { return CurrentPlayer; }
	void SwitchPlayer();

protected:

	void TriggerClick();
	void TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class ATicTacBlock* CurrentBlockFocus;

	Player CurrentPlayer = Player::Player1;

	class ATicTacBoard* Board;
};
