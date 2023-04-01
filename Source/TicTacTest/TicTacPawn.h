#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TicTacPawn.generated.h"

UCLASS()
class TICTACTEST_API ATicTacPawn : public APawn
{
	GENERATED_BODY()

public:
	ATicTacPawn();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	void TriggerClick();
	void TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class ATicTacBlock* CurrentBlockFocus;
};
