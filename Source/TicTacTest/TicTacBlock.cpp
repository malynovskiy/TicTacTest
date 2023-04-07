#include "TicTacBlock.h"
#include "TicTacBoard.h"

ATicTacBlock::ATicTacBlock()
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
    ConstructorHelpers::FObjectFinderOptional<UMaterial> FocusedMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> X_Material;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> O_Material;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Meshes/Cube.Cube"))
			, BaseMaterial(TEXT("/Game/Materials/BaseTicTac.BaseTicTac"))
			, FocusedMaterial(TEXT("/Game/Materials/FocusedMaterial.FocusedMaterial"))
			, X_Material(TEXT("/Game/Materials/X_Material.X_Material"))
			, O_Material(TEXT("/Game/Materials/O_Material.O_Material"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());

	BlockMesh->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
  BlockMesh->SetRelativeLocation(FVector(0.f, 0.f, 25.f));
  BlockMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	BlockMesh->SetMaterial(0, ConstructorStatics.BaseMaterial.Get());
	BlockMesh->OnClicked.AddDynamic(this, &ATicTacBlock::BlockClicked);
	
	BlockMesh->SetupAttachment(DummyRoot);

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
	// prevent clicking on already occupied blocks
	if (bIsActive)
		return;
	
	bIsActive = true;

	ATicTacGameState* gameState = GetWorld()->GetGameState<ATicTacGameState>();
	if (gameState == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ATicTacGameState is null"));
    return;
	}

	const EPlayer currentPlayer = gameState->GetCurrentPlayer();

	if (currentPlayer == EPlayer::Player1)
	{
		BlockMesh->SetMaterial(0, X_Material);
	}
	else
	{
		BlockMesh->SetMaterial(0, O_Material);
	}

  if (OwningBoard != nullptr)
  {
    OwningBoard->HandleMove(index, currentPlayer);
  }
}

void ATicTacBlock::HandleClicked(EPlayer player)
{
	// prevent clicking on already occupied blocks
	if (bIsActive)
		return;

	bIsActive = true;

	if (player == EPlayer::Player1)
	{
		BlockMesh->SetMaterial(0, X_Material);
	}
	else
	{
		BlockMesh->SetMaterial(0, O_Material);
	}

	if (OwningBoard != nullptr)
	{
		OwningBoard->HandleMove(index, player);
	}
}

void ATicTacBlock::Highlight(bool bOn)
{
	if (bIsActive)
		return;

	if (!bOn)
		BlockMesh->SetMaterial(0, BaseMaterial);
	else
		BlockMesh->SetMaterial(0, FocusedMaterial);
}
