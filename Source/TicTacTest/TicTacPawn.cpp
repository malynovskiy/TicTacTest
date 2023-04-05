#include "TicTacPawn.h"
#include "TicTacBlock.h"
#include "TicTacBoard.h"

ATicTacPawn::ATicTacPawn()
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ATicTacPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("TriggerClick", EInputEvent::IE_Pressed, this, &ATicTacPawn::TriggerClick);
}

void ATicTacPawn::StartNewGame()
{
	Board = GetWorld()->SpawnActor<ATicTacBoard>(FVector(0.f, 0.f, 2900.f), FRotator(0, 90.f, 180.f));
}

void ATicTacPawn::EndGame()
{
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

void ATicTacPawn::TriggerClick()
{
	if (CurrentBlockFocus)
		CurrentBlockFocus->HandleClicked();
}
