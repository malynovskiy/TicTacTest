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

	void SetGameBoard(class ATicTacBoard*);
	void SetPlayer(EPlayer player) { Player = player; }

protected:
	virtual void BeginPlay() override;

private:
	void CheckTurnAndMove();

	int32 GetRandMove(TArray<int32> possibleMoves);
	int32 GetNextMove();

private:
	ATicTacGameState* GameState = nullptr;
	class ATicTacBoard* GameBoard = nullptr;

	EPlayer Player = EPlayer::Player2;
};
