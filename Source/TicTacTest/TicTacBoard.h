#pragma once

#include "TicTacPawn.h"
#include "TicTacGameState.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TicTacBoard.generated.h"

UCLASS()
class TICTACTEST_API ATicTacBoard : public AActor
{
	GENERATED_BODY()

	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* PlayerText;

public:
	enum class ECell : int8
	{
		Empty,
		X,
		O
	};

	enum class EWinCondition : int8
	{
		NoWin,
		Player1Win,
		Player2Win,
		Draw
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

	void Initialize(const int32 BoardSize = 3);

	void HandleMove(int32 blockIndex, EPlayer player);

	inline bool IsCellEmpty(int32 index) const { return BoardCells[index] == ECell::Empty; }
	TArray<int32> GetEmptyCells() const;

	bool IsWinningMove(int32 index, EPlayer player) const;

	inline class ATicTacBlock* GetBlock(int32 index) const { return BoardBlocks[index]; }

	void DelayedDestroy() { Destroy(); }

public:
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	FORCEINLINE class UTextRenderComponent* GetPlayerText() const { return PlayerText; }

private:
	void CreateBlocks();
	EWinCondition CheckWinCondition() const;
	bool CheckDrawCondition() const;

	// Array of cells to calculate win condition more efficiently
	TArray<ECell> BoardCells;

	TArray<class ATicTacBlock*> BoardBlocks;

	class ATicTacPawn* OwningPawn;
	class ATicTacGameState* GameState;
};
