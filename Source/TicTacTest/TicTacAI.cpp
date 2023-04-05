#include "TicTacAI.h"

ATicTacAI::ATicTacAI()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ATicTacAI::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATicTacAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ATicTacAI::MakeMove(TArray<int32> possibleMoves)
{
  if (possibleMoves.Num() == 0)
  {
    UE_LOG(LogTemp, Warning, TEXT("GetRandomIndexFromArray: The input array is empty!"));
    return -1;
  }

  // Generate a random index
  int32 RandomIndex = FMath::RandRange(0, possibleMoves.Num() - 1);

  // Return the value at the randomly generated index
  return possibleMoves[RandomIndex];
}

