#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TicTacAI.generated.h"

UCLASS()
class TICTACTEST_API ATicTacAI : public AActor
{
	GENERATED_BODY()
	
public:
	ATicTacAI();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	int32 MakeMove(TArray<int32> possibleMoves);

};
