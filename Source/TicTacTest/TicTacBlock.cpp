#include "TicTacBlock.h"
#include "TicTacBoard.h"

ATicTacBlock::ATicTacBlock()
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> FocusedMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> X_Material;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> O_Material;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
			, BaseMaterial(TEXT("/Game/Puzzle/Meshes/BaseMaterial.BaseMaterial"))
			, FocusedMaterial(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"))
			, X_Material(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"))
			, O_Material(TEXT("/Game/Puzzle/Meshes/GreenMaterial.GreenMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
  /*DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
  RootComponent = DummyRoot;*/

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());

	BlockMesh->SetRelativeScale3D(FVector(1.f, 1.f, 0.25f));
  BlockMesh->SetRelativeLocation(FVector(0.f, 0.f, 25.f));

	BlockMesh->SetMaterial(0, ConstructorStatics.BaseMaterial.Get());
	//BlockMesh->SetupAttachment(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &ATicTacBlock::BlockClicked);

	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	FocusedMaterial = ConstructorStatics.FocusedMaterial.Get();
	X_Material = ConstructorStatics.X_Material.Get();
	O_Material = ConstructorStatics.O_Material.Get();
}

void ATicTacBlock::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATicTacBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATicTacBlock::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}

void ATicTacBlock::HandleClicked()
{
	static int32 counter = 0;
	
	if (bIsActive)
		return;
	
	bIsActive = true;

	// Change material
	if (counter++ % 2 == 0)
		BlockMesh->SetMaterial(0, O_Material);
	else
		BlockMesh->SetMaterial(0, X_Material);

  if (OwningGrid != nullptr)
  {
    OwningGrid->HandlePlayerMove(index);
  }
}

void ATicTacBlock::Highlight(bool bOn)
{
	if (bIsActive)
		return;

	if (bOn)
		BlockMesh->SetMaterial(0, FocusedMaterial);
	else
		BlockMesh->SetMaterial(0, BaseMaterial);
}
