#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TicTacPawn.h"
#include "TicTacBlock.generated.h"


UCLASS()
class TICTACTEST_API ATicTacBlock : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BlockMesh;

public:
	ATicTacBlock();

	bool bIsActive;

	UPROPERTY()
	class UMaterial* BaseMaterial;

	UPROPERTY()
	class UMaterial* FocusedMaterial;

  UPROPERTY()
  class UMaterial* X_Material;

	UPROPERTY()
	class UMaterial* O_Material;

	// TODO: check whether we need public access here
  UPROPERTY()
  class ATicTacBoard* OwningGrid;

public:
	UFUNCTION()
	void BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	void HandleClicked();
	void HandleClicked(EPlayer player);

	void Highlight(bool bOn);

	inline void SetIndex(int32 i) { index = i; };

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return BlockMesh; }

private:
	int32 index;
};
