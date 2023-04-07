#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TicTacGameState.h"
#include "TicTacAI.generated.h"

UCLASS()
class TICTACTEST_API ATicTacAI : public AActor
{
	GENERATED_BODY()
	
public:
	ATicTacAI();

protected:
	virtual void BeginPlay() override;

private:
	void CheckTurnAndMove();
	void MakeMove();

	int32 GetRandMove(TArray<int32> possibleMoves);
	int32 GetNextMove();

	ATicTacGameState* GameState = nullptr;
	class ATicTacBoard* GameBoard = nullptr;
	class ATicTacBlock* BlockToMove = nullptr;
};
