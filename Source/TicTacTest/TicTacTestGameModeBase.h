// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TicTacTestGameModeBase.generated.h"

UCLASS()
class TICTACTEST_API ATicTacTestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATicTacTestGameModeBase();

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> TicTacGameMenu;

	UFUNCTION(BlueprintCallable, Category = "TicTacTest")
  void StartNewGame();

  void InitializeGameMenu();

	virtual void BeginPlay() override;

private:
	class UUserWidget* GameMenu;
};
