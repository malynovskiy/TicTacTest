#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TicTacGameState.h"
#include "TicTacPawn.generated.h"

UCLASS()
class TICTACTEST_API ATicTacPawn : public APawn
{
	GENERATED_BODY()

public:
	ATicTacPawn();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "TicTacTest")
  void StartNewGame(int32 BoardSize, bool isPVE, bool AIsimulation);
	void EndGame();
	
protected:
	void TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class ATicTacBlock* CurrentBlockFocus;

	class ATicTacBoard* Board;

	class ATicTacAI* AIChicky = nullptr; // it should've been Alpha and Bravo, but ... :) 
	class ATicTacAI* AIBricky = nullptr;
};
