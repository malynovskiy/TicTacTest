#include "TicTacBoard.h"
#include "TicTacBlock.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "TicTacBoard"

#define CHECK_WIN_CONDITION(countP1, countP2, Size)                         \
  if (countP1 == Size || countP2 == Size)                                   \
  {                                                                         \
    return countP1 == Size ? EWinCondition::Player1Win : EWinCondition::Player2Win; \
  }

ATicTacBoard::ATicTacBoard()
{
  DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
  RootComponent = DummyRoot;

  PlayerText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
  PlayerText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));
  PlayerText->SetupAttachment(DummyRoot);
}

void ATicTacBoard::BeginPlay()
{
  Super::BeginPlay();

  OwningPawn = dynamic_cast<ATicTacPawn*>(GetWorld()->GetFirstPlayerController()->GetPawn());
  if (OwningPawn == nullptr)
  {
    UE_LOG(LogTemp, Error, TEXT("OwningPawn is null"));
    return;
  }

  GameState = GetWorld()->GetGameState<ATicTacGameState>();
  if (GameState == nullptr)
  {
    UE_LOG(LogTemp, Error, TEXT("ATicTacGameState is null"));
    return;
  }

  PlayerText->SetRelativeLocation(FVector(-270.0f, 0.f, -1800.f));
  PlayerText->SetRelativeRotation(FRotator(90.0f, -180.0f, 0.0f));
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

void ATicTacBoard::Initialize(const int32 BoardSize /*= 3*/)
{
  Size = BoardSize;
  BoardCells.Init(ECell::Empty, Size * Size);

  // Setup location
  FVector CameraLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
  SetActorLocation(FVector(CameraLocation.X, CameraLocation.Y, GetActorLocation().Z));

  CreateBlocks();
}

void ATicTacBoard::CreateBlocks()
{
  // Number of blocks
  const int32 NumBlocks = Size * Size;
  BoardCells.Init(ECell::Empty, NumBlocks);

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
      NewBlock->OwningBoard = this;
      NewBlock->SetIndex(i);

      BoardBlocks.Add(NewBlock);
    }
  }
}

TArray<int32> ATicTacBoard::GetEmptyCells() const
{
  TArray<int32> EmptyCells;
  for (int32 i = 0; i < BoardCells.Num(); i++)
  {
    if (IsCellEmpty(i))
    {
      EmptyCells.Add(i);
    }
  }
  return EmptyCells;
}

bool ATicTacBoard::IsWinningMove(int32 index, EPlayer player) const
{
  if (!IsCellEmpty(index))
    return false;

  TArray<ECell> TestBoard(BoardCells);
  ECell moveCell = player == EPlayer::Player1 ? ECell::X : ECell::O;
  TestBoard[index] = moveCell;

  // horizontal condition check
  for (int32 i = 0; i < Size; i++)
  {
    int32 count{};

    for (int j = 0; j < Size; j++)
    {
      if (TestBoard[i * Size + j] == moveCell)
        count++;
    }

    if (count == Size)
    {
      return true;
    }
  }

  // vertical condition check
  for (int32 j = 0; j < Size; j++)
  {
    int32 count{};

    for (int i = 0; i < Size; i++)
      if (TestBoard[i * Size + j] == moveCell)
        count++;

    if (count == Size)
      return true;
  }

  // diagonals check
  int32 count = 0;
  for (int32 i = 0; i < Size; i++)
    if (TestBoard[i * Size + i] == moveCell)
      count++;

  if (count == Size)
    return true;

  count = 0;
  for (int32 i = 0; i < Size; i++)
    if(TestBoard[i * Size + Size - 1 - i] == moveCell)
      count++;

  if (count == Size)
    return true;

  // no winning move
  return false;
}

void ATicTacBoard::HandleMove(int32 index, EPlayer player)
{
  // Check if selected cell is already occupied BoardCells[index]
  if (!IsCellEmpty(index))
  {
    UE_LOG(LogTemp, Display, TEXT("Selected cell is already occupied by %d"), BoardCells[index]);
    return;
  }

  BoardCells[index] = player == EPlayer::Player1 ? ECell::X : ECell::O;
  GameState->SwitchPlayer();

  EWinCondition winCondition = CheckWinCondition();

  if (winCondition == EWinCondition::NoWin)
  {
    if (CheckDrawCondition())
    {
      winCondition = EWinCondition::Draw;
    }
    else
    {
      return;
    }
  }

  // Game over condition
  FText resultString{};
  EGameState gameState = EGameState::NoWin;
  if (winCondition == EWinCondition::Player1Win)
    resultString = FText::Format(LOCTEXT("ScoreFmt", "Player 1 wins! Score: {0}"), FText::AsNumber(0));
  else if (winCondition == EWinCondition::Player2Win)
    resultString = FText::Format(LOCTEXT("ScoreFmt", "Player 2 wins! Score: {0}"), FText::AsNumber(0));
  else if (winCondition == EWinCondition::Draw)
    resultString = FText::FromString("Draw!");

  gameState = static_cast<EGameState>(winCondition);
  GameState->SetGameState(gameState);
  OwningPawn->EndGame();
}

ATicTacBoard::EWinCondition ATicTacBoard::CheckWinCondition() const
{
  int32 countP1 = 0;
  int32 countP2 = 0;

  auto updateWinCounter = [&countP1, &countP2](ECell cell) {
    if (cell == ECell::X)
      countP1++;
    else if (cell == ECell::O)
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
  return EWinCondition::NoWin;
}

bool ATicTacBoard::CheckDrawCondition() const
{
  for (int i = 0; i < Size * Size; i++)
  {
    if (BoardCells[i] == ECell::Empty)
      return false;
  }
  return true;
}

#undef LOCTEXT_NAMESPACE
