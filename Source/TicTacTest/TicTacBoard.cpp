#include "TicTacBoard.h"
#include "TicTacBlock.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "TicTacBoard"

ATicTacBoard::ATicTacBoard()
{
  PlayerText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
  PlayerText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));

  BoardCells.Init(BoardCell::Empty, Size * Size);
}

void ATicTacBoard::BeginPlay()
{
  Super::BeginPlay();

  APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
  if (PlayerController == nullptr)
  {
    UE_LOG(LogTemp, Error, TEXT("PlayerController is null"));
    return;
  }

  OwningPawn = dynamic_cast<ATicTacPawn*>(PlayerController->GetPawn());
  if (OwningPawn == nullptr)
  {
    UE_LOG(LogTemp, Error, TEXT("OwningPawn is null"));
    return;
  }

  // Setup location
  const float GridHalfSize = (Size * BlockSpacing) / 2;
  FVector CameraLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
  SetActorLocation(FVector(CameraLocation.X, CameraLocation.Y, GetActorLocation().Z));

  CreateBlocks();
}

void ATicTacBoard::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  // cleanup all blocks when game is over
  if (EndPlayReason == EEndPlayReason::Destroyed)
  {
    for (auto block : BoardBlocks)
    {
      block->Destroy();
    }
  }
}

void ATicTacBoard::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

void ATicTacBoard::MakeAIMove()
{

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
void ATicTacBoard::HandlePlayerMove(int32 index, Player player)
{
  // Check if selected cell is already occupied BoardCells[index]
  if (BoardCells[index] != BoardCell::Empty)
  {
    UE_LOG(LogTemp, Display, TEXT("Selected cell is already occupied by %d"), BoardCells[index]);
    return;
  }

  BoardCells[index] = player == Player::Player1 ? BoardCell::X : BoardCell::O;

  //transform 1d index to 2d indices
  const int32 x = index / Size;
  const int32 y = index % Size;

  WinCondition result = CheckWinCondition(x, y);

  if (result == WinCondition::NoWin)
  {
    if (CheckDrawCondition())
    {
      result = WinCondition::Draw;
    }
    else
    {
      // No win, next player turn
      if (OwningPawn != nullptr)
        OwningPawn->SwitchPlayer();
      return;
    }
  }

  // Game over condition
  FText resultString{};
  if (result == WinCondition::Player1Win)
    resultString = FText::Format(LOCTEXT("ScoreFmt", "Player 1 wins! Score: {0}"), FText::AsNumber(0));
  else if (result == WinCondition::Player2Win)
    resultString = FText::Format(LOCTEXT("ScoreFmt", "Player 2 wins! Score: {0}"), FText::AsNumber(0));
  else if (result == WinCondition::Draw)
    resultString = FText::FromString("Draw!");

  OwningPawn->EndGame(result);
  return;
}

TArray<int32> ATicTacBoard::GetEmptyCells() const
{
  TArray<int32> EmptyCells;
  for (int32 i = 0; i < BoardCells.Num(); i++)
  {
    if (BoardCells[i] == BoardCell::Empty)
      EmptyCells.Add(i);
  }
  return EmptyCells;
}

#define CHECK_WIN_CONDITION(countP1, countP2, Size)                         \
  if (countP1 == Size || countP2 == Size)                                   \
  {                                                                         \
    return countP1 == Size ? ATicTacBoard::WinCondition::Player1Win : ATicTacBoard::WinCondition::Player2Win; \
  }

ATicTacBoard::WinCondition ATicTacBoard::CheckWinCondition(int32 x, int32 y) const
{
  int32 countP1 = 0;
  int32 countP2 = 0;

  auto updateWinCounter = [&countP1, &countP2](BoardCell cell) {
    if (cell == BoardCell::X)
      countP1++;
    else if (cell == BoardCell::O)
      countP2++;
  };

  // horizontal condition check
  for (int i = 0; i < Size; i++)
  {
    for (int j = 0; j < Size; j++)
      updateWinCounter(BoardCells[i * Size + j]);

    CHECK_WIN_CONDITION(countP1, countP2, Size);

    countP1 = 0;
    countP2 = 0;
  }

  // vertical condition check
  for (int j = 0; j < Size; j++)
  {
    for (int i = 0; i < Size; i++)
      updateWinCounter(BoardCells[i * Size + j]);

    CHECK_WIN_CONDITION(countP1, countP2, Size);

    countP1 = 0;
    countP2 = 0;
  }

  // diagonals condition check
  for (int i = 0; i < Size; i++)
    updateWinCounter(BoardCells[i * Size + i]);

  CHECK_WIN_CONDITION(countP1, countP2, Size);

  countP1 = 0;
  countP2 = 0;

  for (int i = 0; i < Size; i++)
    updateWinCounter(BoardCells[i * Size + Size - 1 - i]);

  CHECK_WIN_CONDITION(countP1, countP2, Size);

  // No win condition found
  return ATicTacBoard::WinCondition::NoWin;
}

bool ATicTacBoard::CheckDrawCondition() const
{
  for (int i = 0; i < Size * Size; i++)
  {
    if (BoardCells[i] == BoardCell::Empty)
      return false;
  }
  return true;
}

#undef LOCTEXT_NAMESPACE
