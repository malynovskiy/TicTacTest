#pragma once

#include "TicTacPawn.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TicTacBoard.generated.h"

UCLASS()
class TICTACTEST_API ATicTacBoard : public AActor
{
	GENERATED_BODY()

	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* PlayerText;

	using Player = TicTacTest::Player;
	using WinCondition = TicTacTest::WinCondition;

public:
	enum class BoardCell : int32
	{
		Empty,
		X,
		O
	};

	ATicTacBoard();

	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	int32 Size = 3;

	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	float BlockSpacing = 260.f;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;

	void MakeAIMove();

	void HandlePlayerMove(int32 blockIndex, Player player);
	TArray<int32> GetEmptyCells() const;

	FORCEINLINE class UTextRenderComponent* GetPlayerText() const { return PlayerText; }

private:
	void CreateBlocks();
	WinCondition CheckWinCondition(int32 x, int32 y) const;
	bool CheckDrawCondition() const;

	// Array of cells to calculate win condition more efficiently
	TArray<BoardCell> BoardCells;

	TArray<class ATicTacBlock*> BoardBlocks;

	class ATicTacPawn* OwningPawn;
};
