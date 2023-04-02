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
}

UCLASS()
class TICTACTEST_API ATicTacPawn : public APawn
{
	GENERATED_BODY()

public:
	using Player = TicTacTest::Player;

	ATicTacPawn();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "TicTacTest")
  void StartNewGame();

	UFUNCTION(BlueprintCallable, Category = "TicTacTest")
  void QuitGame();

	Player GetCurrentPlayer() const { return CurrentPlayer; }

	void SwitchPlayer();

protected:
	virtual void BeginPlay() override;

	void TriggerClick();
	void TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class ATicTacBlock* CurrentBlockFocus;

	Player CurrentPlayer = Player::Player1;

	class ATicTacBoard* Board;
};
