#include "TicTacPawn.h"
#include "TicTacBlock.h"
#include "TicTacBoard.h"
#include "TicTacAI.h"

ATicTacPawn::ATicTacPawn()
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ATicTacPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATicTacPawn::StartNewGame(int32 BoardSize, bool isPVE, bool AIsimulation)
{
  Board = GetWorld()->SpawnActor<ATicTacBoard>(FVector(0.f, 0.f, -140.f), FRotator(0, 90.f, 180.f));
  Board->Initialize(BoardSize);
	
	ATicTacGameState* gameState = GetWorld()->GetGameState<ATicTacGameState>();
		if (gameState == nullptr)
			return;
	
	gameState->SetGameMode(isPVE ? EGameMode::PVE : EGameMode::PVP);

	if (isPVE)
	{
		AIChicky = GetWorld()->SpawnActor<ATicTacAI>();
		AIChicky->SetGameBoard(Board);
		AIChicky->SetPlayer(EPlayer::Player2);

		if (AIsimulation)
		{
			AIBricky = GetWorld()->SpawnActor<ATicTacAI>();
			AIBricky->SetGameBoard(Board);
			AIBricky->SetPlayer(EPlayer::Player1);
			AIBricky->SetMoveDelay(2.0f);
			
			AIChicky->SetMoveDelay(2.5f);
		}
	}
}

void ATicTacPawn::EndGame()
{
	if (AIChicky != nullptr)
	{
		AIChicky->Destroy();
	}

	if (AIBricky != nullptr)
	{
		AIBricky->Destroy();
	}

	Board->Destroy();

}

void ATicTacPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (playerController == nullptr)
		return;

	FVector Start{}, Dir{};
	playerController->DeprojectMousePositionToWorld(Start, Dir);

	const FVector End = Start + (Dir * 8000.0f);
	TraceForBlock(Start, End, false);
}

void ATicTacPawn::TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers)
{
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);

	if (HitResult.HitObjectHandle.IsValid())
	{
		ATicTacBlock* HitBlock = Cast<ATicTacBlock>(HitResult.GetActor());
		if (CurrentBlockFocus == HitBlock)
			return;
		
		if (CurrentBlockFocus)
			CurrentBlockFocus->Highlight(false);
		
		if (HitBlock)
			HitBlock->Highlight(true);
		
		CurrentBlockFocus = HitBlock;
	}
	else if (CurrentBlockFocus)
	{
		CurrentBlockFocus->Highlight(false);
		CurrentBlockFocus = nullptr;
	}
}
