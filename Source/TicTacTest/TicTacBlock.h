#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	// Material used on focused blocks
	UPROPERTY()
	class UMaterialInstance* FocusedMaterial;

	UPROPERTY()
	class UMaterialInstance* X_Material;

	UPROPERTY()
	class UMaterialInstance* O_Material;

	// TODO: check whether we need public accesss here
  UPROPERTY()
  class ATicTacBoard* OwningGrid;

public:
	UFUNCTION()
	void BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	void HandleClicked();

	void Highlight(bool bOn);

	void inline SetIndex(int32 i) { index = i; };

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return BlockMesh; }

private:
	int32 index;
};
