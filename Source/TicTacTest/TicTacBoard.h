#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TicTacBoard.generated.h"

UCLASS()
class TICTACTEST_API ATicTacBoard : public AActor
{
	GENERATED_BODY()

	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* PlayerText;

public:
	enum class BoardCell : int32
	{
		Empty,
		X,
		O,
		Invalid
	};

	enum class Player : int32
	{
		Player1,
		Player2
	};

	ATicTacBoard();

	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	int32 Size = 8;

	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	float BlockSpacing = 260.f;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void HandlePlayerMove(int32 blockIndex);

	FORCEINLINE class UTextRenderComponent* GetPlayerText() const { return PlayerText; }

private:
	void CreateBlocks();
	int32 CheckWinCondition(int32 x, int32 y) const;

	TArray<BoardCell> BoardCells;
	Player CurrentPlayer = Player::Player1;
};
