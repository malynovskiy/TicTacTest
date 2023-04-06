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

public:
	virtual void Tick(float DeltaTime) override;

	int32 ComputeNextMove(TArray<int32> possibleMoves);

private:
	void CheckTurn();
	void MakeMove();

	ATicTacGameState* GameState = nullptr;
	class ATicTacBoard* GameBoard = nullptr;
	class ATicTacBlock* BlockToMove = nullptr;
};
