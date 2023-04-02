#include "TicTacBoard.h"
#include "TicTacBlock.h"
#include "TicTacPawn.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "TicTacBoard"

ATicTacBoard::ATicTacBoard()
{
	//DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	//RootComponent = DummyRoot;

	// Create static mesh component
  PlayerText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	PlayerText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));
	//PlayerText->SetupAttachment(DummyRoot);

	BoardCells.Init(BoardCell::Empty, Size * Size);
}

void ATicTacBoard::BeginPlay()
{
	Super::BeginPlay();

	const float GridHalfSize = (Size * BlockSpacing) / 2;
	FVector CameraLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	SetActorLocation(FVector(CameraLocation.X, CameraLocation.Y, GetActorLocation().Z));

	CreateBlocks();
}

void ATicTacBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATicTacBoard::CreateBlocks()
{
	// Number of blocks
	const int32 NumBlocks = Size * Size;
	BoardCells.Init(BoardCell::Empty, NumBlocks);

	const auto computeCellLocation = [this](int32 index) -> FVector
	{
		const float XOffset = (index / Size) * BlockSpacing - Size * BlockSpacing / 2 + BlockSpacing / 2;
		const float YOffset = (index % Size) * BlockSpacing - Size * BlockSpacing / 2 + BlockSpacing / 2;

		return FVector(XOffset, YOffset, GetActorLocation().Z);
	};

	for (int32 i = 0; i < NumBlocks; i++)
	{
		const auto location = computeCellLocation(i);

		ATicTacBlock* NewBlock = GetWorld()->SpawnActor<ATicTacBlock>(location, FRotator(0, 0, 0));

		if (NewBlock != nullptr)
		{
			NewBlock->OwningGrid = this;
			NewBlock->SetIndex(i);

      BoardBlocks.Add(NewBlock);
		}
	}
}
void ATicTacBoard::HandlePlayerMove(int32 index)
{
  using Player = ATicTacPawn::Player;

  // Check if selected cell is already occupied BoardCells[index]
  if (BoardCells[index] != BoardCell::Empty)
  {
    UE_LOG(LogTemp, Display, TEXT("Selected cell is already occupied by %d"), BoardCells[index]);
    return;
  }

  // Update board state with player's move

  APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
  if (PlayerController == nullptr)
  {
    UE_LOG(LogTemp, Error, TEXT("PlayerController is null"));
    return;
  }

  ATicTacPawn* ticTacPawn = dynamic_cast<ATicTacPawn*>(PlayerController->GetPawn());
  if (ticTacPawn == nullptr)
  {
    UE_LOG(LogTemp, Error, TEXT("ATicTacPawn is null"));
    return;
  }

  BoardCells[index] = ticTacPawn->GetCurrentPlayer() == Player::Player1 ? BoardCell::X : BoardCell::O;

  //transform 1d index to 2d index  1d index to 2d index
  int32 x = index / Size;
  int32 y = index % Size;

  const int32 result = CheckWinCondition(x, y);
  FText resultString{};
  if (result == 0)
    resultString = FText::Format(LOCTEXT("ScoreFmt", "No win! Score: {0}"), FText::AsNumber(0));
  else if (result == 1)
    resultString = FText::Format(LOCTEXT("ScoreFmt", "Player 1 wins! Score: {0}"), FText::AsNumber(0));
  else if (result == 2)
    resultString = FText::Format(LOCTEXT("ScoreFmt", "Player 2 wins! Score: {0}"), FText::AsNumber(0));

  PlayerText->SetText(resultString);

  ticTacPawn->SwitchPlayer();
}

#define CHECK_WIN_CONDITION(countP1, countP2, Size)                         \
  if (countP1 == Size || countP2 == Size)                                   \
  {                                                                         \
    return countP1 == Size ? 1 : 2; \
  }

int32 ATicTacBoard::CheckWinCondition(int32 x, int32 y) const
{
  int32 countP1 = 0;
  int32 countP2 = 0;

  auto updateWinCounter = [&countP1, &countP2](BoardCell cell) {
    if (cell == BoardCell::X)
      countP1++;
    else if (cell == BoardCell::O)
      countP2++;
  };

  for (int i = 0; i < Size; i++)
  {
    for (int j = 0; j < Size; j++)
      updateWinCounter(BoardCells[i * Size + j]);

    CHECK_WIN_CONDITION(countP1, countP2, Size);

    countP1 = 0;
    countP2 = 0;
  }

  // Check columns
  for (int j = 0; j < Size; j++)
  {
    for (int i = 0; i < Size; i++)
      updateWinCounter(BoardCells[i * Size + j]);

    CHECK_WIN_CONDITION(countP1, countP2, Size);

    countP1 = 0;
    countP2 = 0;
  }

  // Check diagonals
  for (int i = 0; i < Size; i++)
    updateWinCounter(BoardCells[i * Size + i]);

  CHECK_WIN_CONDITION(countP1, countP2, Size);

  countP1 = 0;
  countP2 = 0;

  for (int i = 0; i < Size; i++)
    updateWinCounter(BoardCells[i * Size + Size - 1 - i]);

  CHECK_WIN_CONDITION(countP1, countP2, Size);

  // No win condition found
  return 0;
}

#undef LOCTEXT_NAMESPACE
