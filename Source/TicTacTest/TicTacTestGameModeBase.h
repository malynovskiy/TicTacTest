// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TicTacGameState.h"
#include "TicTacTestGameModeBase.generated.h"

UCLASS()
class TICTACTEST_API ATicTacTestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	using EGameState = TicTacTest::EGameState;

	ATicTacTestGameModeBase();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> TicTacStartGameMenu;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> TicTacEndGameMenu;

protected:
	UFUNCTION(BlueprintCallable, Category = "TicTacTest")
  void StartNewGame();

	UFUNCTION(BlueprintCallable, Category = "TicTacTest")
	void RestartGameMenus();

private:
  void InitializeGameMenus();
  inline void ShowGameMenu(class UUserWidget* gameMenu);
  inline void HideGameMenu(class UUserWidget* gameMenu);

	void OnGameFinished(EGameState gameState);

	class UUserWidget* MainGameMenu;
	class UUserWidget* EndGameMenu;
};
